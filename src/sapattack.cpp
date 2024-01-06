#include "sapattack.hpp"
#include <cmath>
#include <algorithm>

#define PRINT if(true)
SAPAttack::SAPAttack(Dataset* dataset, size_t offset, double alpha, std::string defense , std::map<std::string, std::string> defense_params)
{
    this->name = "SAPAttack";
    this->alpha = alpha;
    this->dataset = dataset;
    this->defense = defense;
    this->offset = offset;
    this->defense_params = defense_params;
}

SAPAttack::~SAPAttack()
{
}

std::vector<std::vector<keyword_id>> SAPAttack::run_attack(std::vector<std::vector<access_pattern>>& query_traces, std::vector<keyword_id> &selected_keywords){

    auto r = process_traces(query_traces);
    std::vector<std::vector<tag>> &observed_tags = r.first;
    std::vector<access_pattern> &tags = r.second;

    // print observed tags
    //log_to_stdout(print_trace(observed_tags, "Observed tags"));

    size_t observation_duration = observed_tags.size();
    size_t num_docs = dataset->document_keywords.size();
    //log_to_stdout("Observation duration: " + std::to_string(observation_duration));
    //log_to_stdout("Number of documents: " + std::to_string(num_docs));
    // calculate tag frequencies

    matrix tag_freq = create_matrix(tags.size(), observation_duration, 0.0);

    for (size_t week = 0; week < observation_duration; week++){

        double number_of_tags_in_week = (double)observed_tags[week].size();
        // normalized tag frequency
        for (auto tag : observed_tags[week]){
            tag_freq[tag][week] += 1 / number_of_tags_in_week;
        }
    }

    //log_to_stdout(print_matrix(tag_freq, "Tag frequencies"));

    // calculate keyword frequencies
    matrix keyword_freq = create_matrix(selected_keywords.size(), observation_duration, 0.0);
    size_t observation_start_week = dataset->trend_weeks - observation_duration - offset;
    for (size_t week = 0; week < observation_duration; week++){
        size_t offset_week = week + observation_start_week;
        double freq_sum = 0;
        for (size_t i = 0; i < selected_keywords.size(); i++){
            keyword_freq[i][week] = dataset->getTrendValue(selected_keywords[i], offset_week);
            keyword_freq[i][week] = clamp(keyword_freq[i][week], 0.00001, 1.0);
            freq_sum += keyword_freq[i][week];
        } 
        // normalize frequency
        for (size_t i = 0; i < selected_keywords.size(); i++){
            keyword_freq[i][week] /= freq_sum;
        }

    }
    
    //(print_matrix(keyword_freq, "Keyword frequencies"));
    // calculate tag volumes
    std::vector<double> tag_volume(tags.size());
    for (tag tag = 0; tag < tags.size(); tag++){
        tag_volume[tag] = tags[tag].size() / (double)num_docs;
    }


    if(this->defense == "CLRZ"){
        // adjusting tag volumes
        double TPR = std::stod(this->defense_params["TPR"]);
        double FPR = std::stod(this->defense_params["FPR"]);
        for (tag tag = 0; tag < tags.size(); tag++){
            tag_volume[tag] = tag_volume[tag] * TPR + (1 - tag_volume[tag]) * FPR;
        }
    }

    //log_to_stdout(print_vector(tag_volume, "Tag volumes"));
    // calculate keyword volumes
    std::vector<double> keyword_volume(selected_keywords.size());
    for (size_t i = 0; i < selected_keywords.size(); i++){
        keyword_volume[i] = dataset->keyword_docs[selected_keywords[i]].size() / (double)num_docs;
        keyword_volume[i] = clamp(keyword_volume[i], 0.00001, 0.99999);
    }

    //log_to_stdout(print_vector(keyword_volume, "Keyword volumes"));
    // calculate frequency matrix
    double freq_weight = 1.0;
    double volume_weight = 1.0;


    matrix freq_matrix = create_matrix(selected_keywords.size(), tags.size(), 0.0);

    for (size_t i = 0; i < selected_keywords.size(); i++){ // for every keyword
        for (size_t j = 0; j < tags.size(); j++){ // for every tag
            double val = 0;
            int k;
            for (k = 0; k < query_traces.size(); k++){ // for every observed week
                val += observed_tags[k].size() * // number of queries in a week
                    tag_freq[j][k] * // tag frequency
                    std::log(keyword_freq[i][k]); // natural log of keyword frequency

            }
            val *= alpha * freq_weight;
            freq_matrix[i][j] = -val;
        }
    }
    //log_to_stdout(print_matrix(freq_matrix, "Frequency matrix"));
    // calculate volume matrix
    
    matrix volume_matrix = create_matrix(selected_keywords.size(), tags.size(), 0.0);
    for (size_t i = 0; i < selected_keywords.size(); i++){ // for every keyword
        for (size_t j = 0; j < tags.size(); j++){ // for every tag
            double val = num_docs * // number of documents
                    tag_volume[j] * // tag volume
                    std::log(keyword_volume[i]) // natural log of keyword volume
                    +
                    num_docs * 
                    (1 - tag_volume[j]) * 
                    (std::log(1 - keyword_volume[i]));
            val *= (1 - alpha) * volume_weight;
            volume_matrix[i][j] = -val;
        }
    }
    //log_to_stdout(print_matrix(volume_matrix, "Volume matrix"));
    matrix cost_matrix = sum(freq_matrix, volume_matrix);
    cost_matrix = transpose(cost_matrix);
    std::vector<int> assignment;
    HungarianAlgorithm hung;
    //std::cout << "hung" << cost_matrix.size() << " " << cost_matrix[0].size() << std::endl;
    double cost = hung.Solve(cost_matrix, assignment);

    //log_to_stdout(print_vector(assignment, "Assignment"));

    std::vector<std::vector<keyword_id>> predicted_query;
    for (auto week_tags : observed_tags){
        std::vector<keyword_id> week_pred(week_tags.size(), 0);
        for (size_t i = 0; i < week_tags.size(); i++){
            week_pred[i] = selected_keywords[assignment[week_tags[i]]];
        }
        predicted_query.push_back(week_pred);
    }
    //log_to_stdout(print_trace(predicted_query, "Predicted query"));

    return predicted_query;
}

std::string SAPAttack::to_string(){
    std::string s = "Attack: " + name + "\n" +
                    "Alpha: " + std::to_string(alpha) + "\n";
    return s;
}