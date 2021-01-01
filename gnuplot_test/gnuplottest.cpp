//#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class GnuplotPipe {
public:
    inline GnuplotPipe(bool persist = true) {
        std::cout << "Opening gnuplot... ";
        pipe = popen(persist ? "gnuplot -persist" : "gnuplot", "w");
        //pipe =popen("\"\"C:\\Program Files\\gnuplot\\bin\\gnuplot -persist\"\"", "w");
        if (!pipe)
            std::cout << "pipe open failed!" << std::endl;
        else
            std::cout << "pipe open succeded." << std::endl;
    }
    inline virtual ~GnuplotPipe(){
        if (pipe) pclose(pipe);
    }

    void sendLine(const std::string& text, bool useBuffer = false){
        if (!pipe) return;
        if (useBuffer)
            buffer.push_back(text + "\n");
        else
            fputs((text + "\n").c_str(), pipe);
    }
    void sendEndOfData(unsigned repeatBuffer = 1){
        if (!pipe) return;
        for (unsigned i = 0; i < repeatBuffer; i++) {
            //for (auto& line : buffer) 
            for( int i=0; i<buffer.size(); ++i)
                fputs(buffer[i].c_str(), pipe);
            fputs("e\n", pipe);
        }
        fflush(pipe);
        buffer.clear();
    }
    void sendNewDataBlock(){
        sendLine("\n", !buffer.empty());
    }

    void writeBufferToFile(const std::string& fileName){
        std::ofstream fileOut;
        fileOut.open(fileName.c_str());
        //for (auto& line : buffer) 
        for( int i=0; i<buffer.size(); ++i)
            fileOut << buffer[i];
        fileOut.close();
    }

private:
    GnuplotPipe(GnuplotPipe const&) = delete;
    void operator=(GnuplotPipe const&) = delete;

    FILE* pipe;
    std::vector<std::string> buffer;
};

using namespace std;

int main(){
    GnuplotPipe gp;
    
    //gp.sendLine("plot [-pi/2:pi] cos(x),-(sin(x) > sin(x+1) ? sin(x) : sin(x+1))");
    gp.sendLine("plot  \"pts.dat\" using 1:2 smooth cspline title 'fx', \\");
    gp.sendLine("\"pts.dat\" using 1:3 smooth cspline title 'ts'");
    
    GnuplotPipe gp1;
    gp1.sendLine("plot [-pi/2:pi] cos(x),-(sin(x) > sin(x+1) ? sin(x) : sin(x+1))");
    
    
    
    return 0;
}
