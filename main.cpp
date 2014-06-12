//
//  main.c
//  OWLConverter
//
//  Created by TJ on 12/06/14.
//  Copyright (c) 2014 TJ. All rights reserved.
//

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>

using namespace std;

ifstream infile;
ofstream outfile;
double threshold=0;

void showInfo(string path){
    cerr<<"Usage:\n"<<path<<" <OWL File> <THRESHOLD>";
    exit(-1);
}

string getStringBetween(string source, string first, string second){
    int begin = source.find(first) + 1;
    int end = source.find(second, begin);
    string ret = source.substr(begin, end-begin);

    //replace special characters
    replace(ret.begin(), ret.end(), ' ', '_'); //space
    ret = regex_replace(ret, regex(">"), "[LOWER_THAN]"); //lower than
    ret = regex_replace(ret, regex("<"), "[GREATER_THAN]"); //greater than
    ret = regex_replace(ret, regex("%"), "[PERCENT]"); //greater than
    return ret;
}

vector<string> nextBlock(){
    string line;
    vector<string> ret;
    if (infile.is_open() && !infile.eof()) {
        do{
            getline(infile, line);
            ret.push_back(line);
        }while (line.compare("\r") != 0 && !infile.eof());
    }
    return ret;
}

bool isOverThreshold(vector<string> block){
    if (block.size() > 1) {
        string temp = getStringBetween(block.at(1), ">", "<");
        double number = stod(temp);
        if(number >= threshold){
            return true;
        }
    }
    
    return false;
}

void convertAndWriteConcept(vector<string> toConvert){
    if (outfile.is_open()) {
        if (isOverThreshold(toConvert)) {
            outfile<<"<owl:Class rdf:about=\"http://www.text2onto.org/ontology#"<<getStringBetween(toConvert.at(2), ">", "<")<<"\"/>"<<endl;
        }
    }
}

void convertAndWriteIndividual(vector<string> toConvert){
    if (outfile.is_open()) {
        if (isOverThreshold(toConvert)) {
            outfile<<"<owl:NamedIndividual rdf:about=\"http://www.text2onto.org/ontology#"<<getStringBetween(toConvert.at(2), ">", "<")<<"\"/>"<<endl;
        }
    }
}

void convertAndWriteRelation(vector<string> toConvert){
    if (outfile.is_open()) {
        if (isOverThreshold(toConvert)) {
            outfile<<"<owl:ObjectProperty rdf:about=\"http://www.text2onto.org/ontology#"<<getStringBetween(toConvert.at(2), ">", "<")<<"\">\n<rdfs:domain rdf:resource=\"http://www.text2onto.org/ontology"<<getStringBetween(toConvert.at(3), "\"", "_c\"")<<"\"/>\n<rdfs:range rdf:resource=\"http://www.text2onto.org/ontology"<< getStringBetween(toConvert.at(4), "\"", "_c\"") <<"\"/>\n</owl:ObjectProperty>"<<endl;
        }
    }
}

string convertBlock(vector<string> toConvert){
    string converted = "";
    if (toConvert.size() > 0 && toConvert.at(0).find("<a:Instance ") == 0) {
        convertAndWriteIndividual(toConvert);
    }else if (toConvert.size() > 0 && toConvert.at(0).find("<a:InstanceOf ") == 0) {
    }else if (toConvert.size() > 0 && toConvert.at(0).find("<a:Concept ") == 0) {
        convertAndWriteConcept(toConvert);
    }else if (toConvert.size() > 0 && toConvert.at(0).find("<a:Relation ") == 0) {
        convertAndWriteRelation(toConvert);
    }
    //TODO else if Concept, Relation, ...
    return converted;
}

void writePrefix(){
    if (outfile.is_open()) {
        outfile<<"<?xml version=\"1.0\"?>\n<!DOCTYPE rdf:RDF [\n<!ENTITY owl 'http://www.w3.org/2002/07/owl#'>\n<!ENTITY xsd \"http://www.w3.org/2001/XMLSchema#\" >\n<!ENTITY rdfs \"http://www.w3.org/2000/01/rdf-schema#\" >\n<!ENTITY rdf \"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" >\n]>\n\n<rdf:RDF\n xml:base=\"http://www.text2onto.org/ontology\"\n xmlns:a=\"http://www.text2onto.org/ontology#\"\n xmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n xmlns:owlx=\"http://www.w3.org/2003/05/owl-xml#\"\n xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\">\n\n<owl:Ontology rdf:about=\"http://www.text2onto.org/ontology\"/>\n\n";
    }
}

void writePostfix(){
    if (outfile.is_open()) {
        outfile<<"</rdf:RDF>";
    }
}

void convert(string filepath){
    vector<string> block;
    cout<<"Start converting "<<filepath<<" with threshold "<<threshold<<endl;
    if(!infile.is_open()) infile.open(filepath);
    if(!outfile.is_open()) outfile.open(filepath.append("_converted.owl"), ofstream::out);
    writePrefix();
    
    do{
        block = nextBlock();
        convertBlock(block);
    }while (block.size() > 0);
    
    writePostfix();
    infile.close();
    outfile.close();
    cout<<"Conversion finished"<<endl;
}

int main(int argc, const char * argv[]){
    if (argc != 3) {
        showInfo(argv[0]);
    }
    
    threshold = stod(argv[2]);
    string filepath = argv[1];
    
    convert(filepath);
    
    return 0;
}

