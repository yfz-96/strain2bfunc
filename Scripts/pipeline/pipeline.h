// Updated at March 8, 2024
// Code by ZHANG Yufeng
// Faculty of dentistry, HKU

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
//#include <omp.h>

#include "utility.h"
#include "version.h"

#define BUFFER_SIZE 5000
//#define TLevN 7
//#define FLevN 4

//#define MAX_BOOT 1000
//#define DEF_BOOT 200

#define MIN_SAM_NUM 3

#define Max(a,b) (((a) > (b)) ? (a) : (b))
#define Min(a,b) (((a) < (b)) ? (a) : (b))

#ifndef PIPELINE_H
#define PIPELINE_H

string path;
string path_2bRADM;
string Bin_path;
string R_path;

//string TLevel[TLevN] = {"phylum", "class", "order", "family", "genus", "species", "OTU"};
//string FLevel[FLevN] = {"l1", "l2", "l3", "KO"};

//bool TLevel_Set[TLevN] = {false};
//bool FLevel_Set[FLevN] = {false};

//Files
//string Id_file;
string Meta_file;
string Seq_list_file;
//string Group_file;
string Taxa_list_file;
string Species_list_file;
string Func_list_file;
string List_prefix;
string Report_file;
string Error_file;
string tmpError_file;

string Table_file;

//Folder
string Abd_dir = "Abundance_Tables";
string Dist_dir = "Distance_Matrix";
string Clust_dir = "Clustering";
string Marker_dir = "Markers";
string Network_dir = "Network";
string Alpha_dir = "A_Diversity";
string Beta_dir = "B_Diversity";
//string Sampleview_dir = "Sample_Views";
//string Singlesample_dir = "Single_Sample";
//string Singlesamplerare_dir = "Single_Sample.Rare";
//string Singlesamplelist_dir = "Single_Sample.List";
string Temp_dir = "Temp";

//para
char Ref_db = 'G'; //G: GG97; S: SILVA 16s; O Oral_Core; E: SILVA 18S; T: ITS; C: GG99
//_PMDB Database;

string Out_path;
char Seq_type = 'r';

int Length_t = 0;
int Cluster = 2;
float Network_t = 0.5;
char Is_pair = 'F';
char Is_format_check = 'F';
char Is_cp = 'T';
float sp_thres;

bool Is_taxa = true;
bool Is_func = true;
bool Is_rare = false;
bool Is_rare_curve = false;
bool Is_paired_seq = false;

//newadd
//char Is_denoised='T';
//char Is_nonchimeras='T';
//double db_similarity=0.99;

//int Rare_depth = 0;
//int Bootstrap = DEF_BOOT;

//step
int Step = -1;

//Input mode for step 2
int Mode = 0; //0: Multiple-species separated; 1: Multiple-species merged

int Taxa_dist_type = 0;
//string Taxa_dist_name = "Meta-Storms";

int Coren = 0;

vector <string> Seq_files;
vector <string> Ids;

//Sample info
int Input_sam_num = 0;
int Filter_sam_num = 0;

string prefix_name = "default"; //prefix of analysis-results-file name

int printhelp(){
    
    cout << "Welcome to Strain2bFunc Pipeline" << endl;
    cout << "Version: " << Version << endl;
    cout << "Usage: " << endl;
    cout << "Strain2bFunc-pipeline [Option] Value" << endl;
    cout << "Options: " << endl << endl;
    
    cout << "\t-D (upper) ref database" << endl;
    //cout << "\t-D (upper) ref database, " << _PMDB::Get_Args() << endl;
    cout << "\t-m Meta data file [Required]" << endl;
    cout << endl;
    
    cout << "\t[Input options, required]" << endl;
    
    cout << "\tStart from step0: Species-level profiling" << endl;
    cout << "\t  -i Sequence files list, 2bRAD or WGS data [Conflicts with -l, -T and -L]" << endl;
    cout << "\tor" << endl;
    cout << "\tStart from step1: Automatically select species" << endl;
    cout << "\t  -l 2bRAD sequence files list [Conflicts with -i]" << endl;
    cout << "\t  -T (upper) Input Species-level abundance table generated by 2bRAD-M [Conflicts with -i and -L]" << endl;
    cout << "\tor" << endl;
    cout << "\tStart from step2: Strain-level profiling" << endl;
    cout << "\t  -l 2bRAD sequence files list [Conflicts with -i]" << endl;
    cout << "\t  -L Species list (each row is a species) [Conflicts with -i and -T]" << endl;
    cout << "\t  -M (upper) Input the Mode for strain-level analysis, 0 for multiple-species separated analysis, 1 for multiple-species merged analysis, default is 0 [Optional for -L]" << endl;
    cout << "\t  -p List file path prefix [Optional for -i, -T and -L]" << endl;

    cout << endl;
    
    cout << "\t[Output options]" << endl;
    cout << "\t  -o Output path, default is \"default_out\"" << endl;
    cout << endl;
    
    cout << "\t[Profiling parameters]" << endl;
    cout << "\t  -S (upper) Sequence type, T (2bRAD) or F (Shotgun), default is T" << endl;
    cout << "\t  -a the abundance threshold of species for Strain2bFunc analysis, default is 0.01" << endl;
    //cout << "\t  -k Sequence format check, T(rue) or F(alse), default is F" << endl;
    cout << "\t  -f Functional analysis, T(rue) or F(alse), default is T" << endl;
    //newadd
    //cout << "\t  -v ASV denoising, T(rue) or F(alse), default is T [optional for -i]" << endl;
    //cout << "\t  -c Chimera removal, T(rue) or F(alse), default is T [optional for -i]" << endl;
    //cout << "\t  -d Sequence alignment threshold (float value 0~1), default is 0.99 for ASV enabled and 0.97 for ASV disabled (-v F) [optional for -i]" << endl;
    cout << endl;
    
    cout << "\t[Statistic parameters]" << endl;
    //cout << "\t  -L (upper) Taxonomical levels (1-6: Phylum - Species). Multiple levels are accepted" << endl;
    cout << "\t  -w Taxonomical distance type, 0: Bray-Curtis, 1: Euclidean, 2: Jaccard, default is 0" << endl;
    //cout << "\t  -F (upper) Functional levels (Level 1, 2, 3 or 4 (KO number)). Multiple levels are accepted" << endl;
    //cout << "\t  -s Sequence number normalization depth, 0 is disabled, default is disable" << endl;
    //cout << "\t  -b Bootstrap for sequence number normalization, default is " << DEF_BOOT << ", maximum is " << MAX_BOOT << endl;
    //cout << "\t  -R (upper) Rarefaction curve, T(rue) or F(alse), default is F" << endl;
    //cout << "\t  -E (upper) If the samples are paired, T(rue) or F(alse), default is F" << endl;
    cout << "\t  -C (upper) Cluster number, default is 2" << endl;
    cout << "\t  -G (upper) Network analysis edge threshold, default is 0.5" << endl;
    cout << endl;
    
    cout << "\t[Other options]" << endl;
    cout << "\t  -t Number of thread, default is auto" << endl;
    cout << "\t  -h help" << endl;
    
    exit(0);
    
    return 0;
    
    };

void Print_Report(const char * outfilename){
     
    ofstream outfile(outfilename, ios::out);
    if(!outfile){
              cerr << "Error: Cannot open report file: " << outfilename << endl;
              return;
              }

    outfile << "Strain2bFunc Pipeline Analysis Report" << endl;
    outfile << "Version " << Version << endl;

    outfile << "Reference sequence database: ";
    //outfile << Database.Get_Description() << endl; // GTDB is default

    if (Step == 0){ //Profiling info
          outfile << "Input type: Sequence list" << endl;
          outfile << "Sequence list: " << Seq_list_file << endl;
          outfile << "Sequence type: ";
          if (Seq_type == 'F') outfile << "Metagenomic shotgun sequences" << endl;
          else outfile << "2bRAD sequences" << endl;
    }

    outfile << "The abundance threshold of species for Strain2bFunc analysis: ";
    outfile <<  sp_thres <<endl;


    outfile << "Functional anlaysis: ";
    if (Is_func) outfile << "Yes" << endl;
    else outfile << "No" << endl;

    outfile << "Taxonomy distance: ";
    if (Taxa_dist_type == 0) outfile << "Bray-Curtis distance" << endl;
    if (Taxa_dist_type == 1) outfile << "Euclidean distance" << endl;
    if (Taxa_dist_type == 2) outfile << "Jaccard distance" << endl;

    //if ((Taxa_dist_type == 0) || (Taxa_dist_type == 2)) outfile << "Weighted taxa distance: " << Taxa_dist_name << endl;
    //if ((Taxa_dist_type == 1) || (Taxa_dist_type == 2)) outfile << "Unweighted taxa distance: " << Taxa_dist_name_uw << endl;
    //outfile << "Weighted function distance: ";
    //outfile << "Function distance: ";
    //if (Is_func) outfile << Func_dist_name << endl;
    //else outfile << "NA" << endl;
       
    outfile << "Number of Input Sample: " << Input_sam_num << endl;
    outfile << "Number of Output Sample: " << Filter_sam_num << endl;

    outfile.close();
    outfile.clear();
    
}

int Parse_Para(int argc, char * argv[]){

    Bin_path = "";
    R_path = Check_Env() + "/Rscript";
    path = Check_Env();
    path_2bRADM = path + "/Scripts/2bRAD-M";
    //path_2bRADM = Check_2bRADM_Env();
    
    Ref_db = 'G';
    
    List_prefix = "";
    
    Step = -1;
    Cluster = 2;
    Seq_type = 'T';
    Coren = 0;
    Out_path = "default_out/";
    
    //init TLevel & FLevel
    //TLevel_Set[0] = true; //phylum
    //TLevel_Set[4] = true; //genus
    //TLevel_Set[6] = true; //OTU
    
    //FLevel_Set[1] = true; //l2
    //FLevel_Set[2] = true; //l3
    
    int i = 1;
      
     if (argc ==1)
        printhelp();
      
      while(i<argc){
          
         if (argv[i][0] != '-') {
            cerr << "Argument # " << i << " Error : Arguments must start with -" << endl;
            exit(0);
         };
          
          switch(argv[i][1]){
                  //basic args
                  //case 'D': Database.Set_DB(argv[i+1][0]); Ref_db = Database.Get_Id(); break;
                  
              case 'm': Meta_file = argv[i+1]; break;
                  
              case 'i': Seq_list_file = argv[i+1];
                  if (Step == 1) {
                      cerr << "Error: -i conflicts with -l and -T" << endl;
                      exit(0);
                  } else if(Step == 2) {
                      cerr << "Error: -i conflicts with -l and -L" << endl;
                      exit(0);
                  }
                  else Step = 0;
                  break;
                  
              case 'l': Taxa_list_file = argv[i+1];
                  if (Step == 0) {
                      cerr << "Error: -l conflicts with -i" << endl;
                      exit(0);
                  }
                  break;
                  
              case 'T': Table_file = argv[i+1];
                  if (Step == 0) {
                      cerr << "Error: -T conflicts with -i" << endl;
                      exit(0);
                  } else if (Step == 2) {
                      cerr << "Error: -T conflicts with -L" << endl;
                      exit(0);
                  }
                  else Step = 1;
                  break;
                  
              case 'L': Species_list_file = argv[i+1];
                  if (Step == 0) {
                      cerr << "Error: -L conflicts with -i" << endl;
                      exit(0);
                  }
                  else if (Step == 1) {
                      cerr << "Error: -L conflicts with -T" << endl;
                      exit(0);
                  }
                  else Step = 2;
                  break;
                  
              case 'M': Mode = atoi(argv[i+1]); break;
                  
              case 'p': List_prefix = argv[i+1]; break;
                  
              case 'o': Out_path = argv[i+1]; break;
                  
              case 'S': if ((argv[i+1][0] == 'T') || (argv[i+1][0]== 't')) {
                            Seq_type = 'T';
                        } else {
                            Seq_type = 'F';
                        }
                  break;
                  
              case 'a': sp_thres = atof(argv[i+1]); break;
                  
              case 'f': if ((argv[i+1][0] == 'f') || (argv[i+1][0] == 'F' )) Is_func = false; break;
                  
              case 'w':
              case 'W': Taxa_dist_type = atoi(argv[i+1]);
                  if ((Taxa_dist_type < 0) || (Taxa_dist_type > 2)) Taxa_dist_type = 2;
                  break;
                  
              case 'C': Cluster = atoi(argv[i+1]); break;
                  
              case 'G': Network_t = atof(argv[i+1]); break;
                  
                  //other args
              case 't': Coren = atoi(argv[i+1]); break;
                  
              case 'h': printhelp(); break;
                  
              default: cerr << "Error: Unrec argument " << argv[i] << endl; printhelp(); break;
          }
        
          i += 2;
    }
    
    if ((Step > 2) || (Step < 0)){
              cerr << "Warning: Step (-s) must be 0 - 2, change to default (0)" << endl;
              Step = 0;
    }
    
    Abd_dir = Out_path + "/analysis_results/Abundance_Tables";
    Dist_dir = Out_path + "/analysis_results/Distance_Matrix";
    Clust_dir = Out_path + "/analysis_results/Clustering";
    Marker_dir = Out_path + "/analysis_results/Markers";
    Network_dir = Out_path + "/analysis_results/Network";
    Alpha_dir = Out_path + "/analysis_results/Alpha_Diversity";
    Beta_dir = Out_path + "/analysis_results/Beta_Diversity";
    //Sampleview_dir = Out_path + "/Sample_Views";
    //Singlesample_dir = Out_path + "/Single_Sample";
    //Singlesamplerare_dir = Out_path + "/Single_Sample.Rare";
    //Singlesamplelist_dir = Out_path + "/Single_Sample.List";
    Temp_dir = Out_path + "/Temp";
    
    Report_file = Out_path + "/Analysis_Report.txt";
    Error_file = Out_path + "/error.log";
    tmpError_file= Temp_dir + "/tmperror.log";
    remove(Error_file.c_str());
    
    int Max_Core_number = 8; //////////////////sysconf(_SC_NPROCESSORS_CONF);
    
    if ((Coren <= 0) || (Coren > Max_Core_number)){
                    //cerr << "Core number must be larger than 0, change to automatic mode" << endl;
                    Coren = Max_Core_number;
                    }
    //if (Rare_depth <= 0) Is_rare = false;
    /*
    if (Bootstrap <= 0){
        cerr << "Warning: The minimum bootstrap is 1, change to default " << DEF_BOOT << endl;
        Bootstrap = DEF_BOOT;
    }
    if (Bootstrap > MAX_BOOT) {
        cerr << "Warning: The maximum bootstrap is " << MAX_BOOT << ", change to default " << DEF_BOOT << endl;
        Bootstrap = DEF_BOOT;
        }
     */
    /*
    if (!Database.Get_Is_Func()) Is_func = false;
    if (!Database.Get_Is_Cp()) Is_cp = 'F';
    if (!Database.Get_Is_Tree()){
                                  Taxa_dist_type = 0;
                                  Taxa_dist_name = "Jensen-Shannon";
                                  }
     */
    return 0;
     }
void Run_With_Error(char * command, const char * error){
     
     string command_with_error;
     command_with_error = command;
     command_with_error +=" 2>>";
     command_with_error += error;
     system(command_with_error.c_str());
     }

void Echo_Error(const char * error_info, const char * error){
     
     string command_with_error;
     command_with_error = "echo \"";
     command_with_error += error_info;
     command_with_error += "\" >>";
     command_with_error += error;
     system(command_with_error.c_str());
     }

void Run_With_Error(const char * command, const char * program, const char * error){
     
     string command_with_error;
     //make error title
     command_with_error = "echo \"";
     command_with_error += program;
     command_with_error += ":\" >>";
     command_with_error += error;
     system(command_with_error.c_str());
     
     //make command
     command_with_error = command;
     command_with_error +=" 2>>";
     command_with_error += error;
     system(command_with_error.c_str());
     }

bool Check_Ids(vector <string> ids){
     
     if (ids.size() ==0) return false;
     for (int i = 0; i < ids.size(); i ++)
         if ((ids[i][0] >= '0') && (ids[i][0] <= '9')) //started by number
            return false;
     return true;
     }

void Check_Ids_By_Path(const char * path, vector <string> & ids){
    //check ids
    vector <string> newids;
    newids.push_back(ids[0]); //title
    
    for (int i = 0; i < ids.size(); i ++){
        string a_path = path;
        a_path += "/";
        a_path += ids[i];
        if (Check_Path(a_path.c_str())){
            newids.push_back(ids[i]);
        }
    }
    
    if (ids.size() != newids.size())
        ids = newids;

    }

bool Check_Metadata_By_Ids(vector <string> ids, string & metafile, string outmetafile){
    
    //update metadata
    ifstream infile(metafile.c_str(), ifstream::in);
    if (!infile){
        cerr << "Error: Cannot open input metadata file: " << metafile << endl;
        return false;
    }
    
    ofstream outfile(outmetafile.c_str(), ofstream::out);
    if (!outfile){
        cerr << "Error: Cannot open output metadata file: " << outmetafile << endl;
        return false;
    }

    map <string, string> metadata;
    string buffer;
    getline(infile, buffer); //title
    outfile << buffer << endl; //title
    while(getline(infile, buffer)){
        stringstream strin(buffer);
        string id;
        strin >> id;
        metadata[id] = buffer;
        }
    
    for (int i = 0; i < ids.size(); i ++){
        
        if (metadata.count(ids[i]))
            outfile << metadata[ids[i]] << endl;
        else{
            cerr << "Error: Cannot find metadata for sample: " << ids[i] << endl;
            return false;
            }
        }
    
    infile.close();
    infile.clear();
    outfile.close();
    outfile.clear();
    
    metafile = outmetafile;
    
    return true;
    }
/*
void Copy_Index(const char * path){
     //cp html
     string command;
     command = "cp ";
     command += Check_Env();
     command += "/html/index.html ";
     command += path;
     
     system(command.c_str());
     //cp resources of html
     string command2;
     command2 = "cp -rf ";
     command2 += Check_Env();
     command2 += "/html/PageResources ";
     command2 += path;

     system(command2.c_str());
     }
    */
#endif
