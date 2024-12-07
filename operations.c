#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 2048
#define MAX_ENTRIES 5000

// Define structure for county data
typedef struct {
    char county[100];
    char state[3];
    float education_high_school;
    float education_bachelors_or_higher;
    float ethnicities[10];
    int income_median_household;
    int income_per_capita;
    float income_below_poverty;
    int population_2014;
} CountyData;

// Global variables for county data
CountyData data[MAX_ENTRIES];
int data_count = 0;

// Function prototypes

void process_operations_file(const char *filename);
void display_data(int data_count, CountyData *data);
void filter_state(const char *state);
void filter_field(const char *field, const char *op, float value);
void calculate_population_total();
void calculate_subpopulation(const char *field);
void calculate_percentage(const char *field);
int is_valid_field(const char *field, float *value, CountyData entry);

#include <string.h>
#include <_string.h>

// Utility function to remove quotes from strings
void strip_quotes(char *str) {
    size_t len = strlen(str);
    if (len > 1 && str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 1); // Shift string to the left
        str[len - 2] = '\0';           // Null-terminate after removing last quote
    }
}

// Load demographics file
int load_demographics_file(const char *filename, CountyData *data) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open demographics file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    int line_num = 0;

    // Skip header
    fgets(line, MAX_LINE, file);

    // Read data
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        int columns = 0;
        int column_count = 0;
        CountyData entry;
        char *token = strtok(line, ",");
        //printf("\tnew line of file\n");
        while(token){
            //printf("\t\t before strip %s\n", token);
            strip_quotes(token);
            //printf("\t\t  after strip %s\n", token);
            switch (columns){
                case 0:
                    strncpy(data[line_num].county, token, sizeof(data[line_num].county)-1);
                    data[line_num].county[sizeof(data[line_num].county)-1] = '\0';
                    column_count++;
                    //printf(" county %s\n",data[line_num].county );
                    break;

                case 1:
                    strncpy(data[line_num].state, token, sizeof(data[line_num].state)-1);
                    data[line_num].state[sizeof(data[line_num].state)-1] = '\0';
                    column_count++;
                    //printf("%s\n",data[line_num].state );
                    break;
                case 5:
                    if(sscanf(token, "%f", &data[line_num].education_bachelors_or_higher)==0){
                        fprintf(stderr, "not able to parse bachelor data\n");
                    }
                    column_count++;
                    //printf(" education bachelors %f\n",data[line_num].education_bachelors_or_higher );
                    break;
                case 6:
                    if(sscanf(token, "%f", &data[line_num].education_high_school)==0){
                        fprintf(stderr, "not able to parse highschool data\n");
                    }
                    column_count++;
                    //printf(" education hs%f\n",data[line_num].education_high_school );
                    break;
                 case 11:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[0])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.American Indian\n");
                    }else{
                    column_count++;
                    //printf(" ethnicity native %f\n",data[line_num].ethnicities[0] );
                    }
                    break;
                case 12:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[1])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.Asian\n");
                    }else{
                    column_count++;
                    //printf(" asian %f\n",data[line_num].ethnicities[1] );
                    }
                    break;
                case 13:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[2])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.Black\n");
                    }else{
                    column_count++;
                    //printf(" black%f\n",data[line_num].ethnicities[2] );
                    }
                    break;
                case 14:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[3])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.Hispanic\n");
                    }else{
                    column_count++;
                    //printf(" hispanic %f\n",data[line_num].ethnicities[3] );
                    }
                    break;
                case 15:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[4])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.Native Hawiian or pacific islander\n");
                    }else{
                    column_count++;
                    //printf(" hawaiian %f\n",data[line_num].ethnicities[4] );
                    }
                    break;
                case 16:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[5])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.Two or more\n");
                    }else{
                    column_count++;
                    //printf(" two or more %f\n",data[line_num].ethnicities[5] );
                    }
                    break;
                case 17:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[6])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.White ALone\n");
                    }else{
                    column_count++;
                    }
                    break;
                case 18:
                    if(sscanf(token, "%f", &data[line_num].ethnicities[7])==0){
                        fprintf(stderr, "not able to parse: Ethinicites.White Alone not Hispanic\n");
                    }else{
                    column_count++;
                    }
                    break;
                case 25:
                    if(sscanf(token, "%d", &data[line_num].income_median_household)==0){
                        fprintf(stderr, "not able to parse: Income.Median\n");
                    }else{
                    column_count++;
                    }
                    break;
                case 26:
                    if(sscanf(token, "%d", &data[line_num].income_per_capita)==0){
                        fprintf(stderr, "not able to parse: Income.Per Capita\n");
                    }else{
                    column_count++;
                    }
                    break;
                case 27:
                    if(sscanf(token, "%f", &data[line_num].income_below_poverty)==0){
                        fprintf(stderr, "not able to parse: Income:Below Poverty\n");
                    }else{
                    column_count++;
                    }
                    break;
                case 38:
                    if(sscanf(token, "%d", &data[line_num].population_2014)==0){
                        fprintf(stderr, "not able to parse: Population.2014\n");
                    }else{
                    column_count++;
                    }
                    break;


            }

            token = strtok(NULL, ",");
            columns++;

        }
        if (column_count == 16){
            line_num++;
        }
        
        
        // Add valid entry to data array
        if (data_count < MAX_ENTRIES) {
            data[data_count++] = entry;
        } else {
            fprintf(stderr, "Error: Maximum number of entries exceeded.\n");
            break;
        }

    }

    fclose(file);
    printf("Loaded %d entries from demographics file.\n", data_count);
    return 0;
    

}

// Process operations file
void process_operations_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open operations file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE];
    int line_num = 0;

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        char *operation = strtok(line, ":");
        if (!operation || operation[0] == '\n') continue;

        if (strcmp(operation, "display\n") == 0) {
            display_data(data_count, data);
        } else if (strcmp(operation, "filter-state") == 0) {
            char *state = strtok(NULL, ":");
            if (!state) {
                fprintf(stderr, "Error: Malformed operation on line %d.\n", line_num);
                continue;
            }
            filter_state(state);
        } else if (strcmp(operation, "filter") == 0) {
            char *field = strtok(NULL, ":");
            char *op = strtok(NULL, ":");
            char *value_str = strtok(NULL, ":");
            if (!field || !op || !value_str) {
                fprintf(stderr, "Error: Malformed operation on line %d.\n", line_num);
                continue;
            }
            float value = atof(value_str);
            filter_field(field, op, value);
        } else if (strcmp(operation, "population-total") == 0) {
            calculate_population_total();
        } else if (strcmp(operation, "population") == 0) {
            char *field = strtok(NULL, ":");
            if (!field) {
                fprintf(stderr, "Error: Malformed operation on line %d.\n", line_num);
                continue;
            }
            calculate_subpopulation(field);
        } else if (strcmp(operation, "percent") == 0) {
            char *field = strtok(NULL, ":");
            if (!field) {
                fprintf(stderr, "Error: Malformed operation on line %d.\n", line_num);
                continue;
            }
            calculate_percentage(field);
        } else {
            fprintf(stderr, "Error: Unknown operation '%s' on line %d.\n", operation, line_num);
        }
    }

    fclose(file);
    data_count = line_num;
    //return line_num;
}

// Display all data

void display_data(int entries, CountyData *data){
    int n;

    for(n = 0; n < entries; n++){
        printf("first line\n");
        printf("%s\n", data[0].county);
        printf("%s, %s\n", data[n].county, data[n].state);
        printf("\tEducation:\n\t\tBachelor's Degree or Higher: %.1f%%\n\t\tHigh School or Higher: %.1f%%\n", data[n].education_bachelors_or_higher, data[n].education_high_school);
        printf("\tEthnicities:\n");
        printf("\t\tAmerican Indian and Alaska Native Alone: %.1f%%\n", data[n].ethnicities[0]);
        printf("\t\tAsian Alone: %.1f%%\n", data[n].ethnicities[1]);
        printf("\t\tBlack Alone: %.1f%%\n", data[n].ethnicities[2]);
        printf("\t\tHispanic or Latino: %.1f%%\n", data[n].ethnicities[3]);
        printf("\t\tNative Hawaiian and Other Pacific Islander Alone: %.1f%%\n", data[n].ethnicities[4]);
        printf("\t\tTwo or More Races: %.1f%%\n", data[n].ethnicities[5]);
        printf("\t\tWhite Alone: %.1f%%\n", data[n].ethnicities[6]);
        printf("\t\tWhite Alone not Hispanic or Latino: %.1f%%\n", data[n].ethnicities[7]);
        printf("\tIncome:\n\t\tMedian Household Income: %d\n", data[n].income_median_household);
        printf("\t\tPer Capita Income: %d\n", data[n].income_per_capita);
        printf("\t\tPersons Below Poverty Level: %.1f%%\n", data[n].income_below_poverty);
        printf("\t2014 Population: %d\n\n", data[n].population_2014);
    }
    printf("printed %d entries ...\n", entries);
    return;
}


// Filter by state
void filter_state(const char *state) {
    int count = 0;
    for (int i = 0; i < data_count; i++) {
        if (strncmp(data[i].state, state, 2) == 0) {
            count= count + 1;
        }
    }
    printf("Filter: state == %s (%d entries)\n", state, count);
}

// Filter by field
void filter_field(const char *field, const char *op, float value) {
    int count = 0;
    for (int i = 0; i < data_count; i++) {
        float field_value = 0;
        if (is_valid_field(field, &field_value, data[i])) {
            if ((strcmp(op, "ge") == 0 && field_value >= value) ||
                (strcmp(op, "le") == 0 && field_value <= value)) {
                data[count++] = data[i];
            }
        }
    }
    data_count = count;
    printf("Filter: %s %s %.2f (%d entries)\n", field, op, value, data_count);
}

// Calculate total population
void calculate_population_total() {
    int total = 0;
    for (int i = 0; i < data_count; i++) {
        total += data[i].population_2014;
        
    }
    printf("2014 population: %d\n", total);
}

// Calculate subpopulation
void calculate_subpopulation(const char *field) {
    int total = 0;
    for (int i = 0; i < data_count; i++) {
        float percentage = 0;
        if (is_valid_field(field, &percentage, data[i])) {
            total += (int)(data[i].population_2014 * (percentage / 100));
        }
    }
    printf("2014 %s population: %d\n", field, total);
}

// Calculate percentage
void calculate_percentage(const char *field) {
    int sub_total = 0, total_population = 0;
    for (int i = 0; i < data_count; i++) {
        float percentage = 0;
        if (is_valid_field(field, &percentage, data[i])) {
            sub_total += (int)(data[i].population_2014 * (percentage / 100));
        }
        total_population += data[i].population_2014;
    }
    printf("2014 %s percentage: %.2f%%\n", field, (100.0 * sub_total) / total_population);
}

// Validate field and get value
int is_valid_field(const char *field, float *value, CountyData entry) {
    if (strcmp(field, "Education.High School or Higher") == 0) {
        *value = entry.education_high_school;
        return 1;
    } else if (strcmp(field, "Education.Bachelor's Degree or Higher") == 0) {
        *value = entry.education_bachelors_or_higher;
        return 1;
    } else if (strcmp(field, "Income.Persons Below Poverty Level") == 0) {
        *value = entry.income_below_poverty;
        return 1;
    }
    return 0;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <demographics_file> <operations_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    load_demographics_file(argv[1], data);
    process_operations_file(argv[2]);

    return EXIT_SUCCESS;
}
