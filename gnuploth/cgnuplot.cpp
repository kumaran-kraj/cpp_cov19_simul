//#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include<math.h>

class GnuplotPipe {
private:
    GnuplotPipe(GnuplotPipe const&) = delete;
    void operator=(GnuplotPipe const&) = delete;

    FILE* pipe;
    std::vector<std::string> buffer;
    //std::string dat_file="pts.dat";
    std::string g_name;
    std::ofstream ptsout;
    
public:
    std::string dat_file="pts.dat";
    GnuplotPipe(std::string graphName="Graph1",bool persist = true) 
    {
        
        dat_file=graphName+".dat";
        g_name=graphName;
        pipe=NULL;
        //pipe
        /*
        std::cout << "\n Opening gnuplot... ";
        pipe = popen(persist ? "gnuplot -persist" : "gnuplot", "w");
        if (!pipe)
            std::cout << " pipe open failed!" << std::endl;
        else
            std::cout << " pipe open succeded." ;//<< std::endl;
        //pipe
        */
        openPipe(persist);
        
        ptsout.open(dat_file.c_str());
        ptsout<<"#\n";
        ptsout.close();
        
    }// constructor creates pipe
    void openPipe(bool persist = true)
    {
        if (!pipe)
        {
            std::cout << "\n Opening gnuplot... ";
            pipe = popen(persist ? "gnuplot -persist" : "gnuplot", "w");
        }
        if (!pipe)
            std::cout << "\n pipe open failed!" << std::endl;
        else
            std::cout << "\n pipe open succeded." ;//<< std::endl;
         
    }
    void closePipe()
    {
        std::cout<<"\n close pipe";
        if (pipe)
        { 
            pclose(pipe);
            pipe=NULL;
        }
    }
    

    void sendLine(const std::string& text, bool useBuffer = false){
        std::cout<<"\n sendline";
        if (!pipe)
        {
            std::cout<<"\n no pipe -sendline";
            return;
        }
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
    
    void writePointsBlock(std::vector<std::vector<long double> > pts)
    {
        //std:ofstream ptsout;
        ptsout.open(dat_file.c_str(),std::ios::out | std::ios::app);
        for( int i=0; i<pts.size(); ++i)
        {
            for( int j=0; j<pts[i].size(); ++j)
            {
                ptsout<<pts[i][j]<<" ";
            }
             ptsout<<"\n";
        }
        ptsout.close();
    }
    void writePoints(std::vector<long double> pts)
    {
        //std:ofstream ptsout;
        ptsout.open(dat_file.c_str(),std::ios::out | std::ios::app);
        
            for( int j=0; j<pts.size(); ++j)
            {
                ptsout<<pts[j]<<" ";
            }
         ptsout<<"\n";
        ptsout.close();
    }
    
    inline virtual ~GnuplotPipe(){
        //if (pipe) pclose(pipe);
        remove( dat_file.c_str() );
        std::cout<<"\n gnupipe closed";
    }// destructor closes pipe , easy to work with as scope takes care of pipe deletion

};

class gnugraph
{
    GnuplotPipe gp;
    std::string g_name;
    std::string sc_name;
    std::ofstream plot_script;
public:
    gnugraph(std::string graph_name="sc")
    {
        g_name=graph_name;
        sc_name=g_name+".p";
        std::cout<<"\n graph start";
        
        plot_script.open(sc_name.c_str());
        plot_script<<"#\nplot x**2 \n";
        plot_script.close();
    }
    void show ()
    {
        std::cout<<"\n-------------";
        gp.openPipe();
        gp.sendLine("load \""+sc_name+"\"");
        std::cout<<"\n plotting";
        gp.closePipe();
        std::cout<<"\n-------------";
    }
    ~gnugraph()
    {
        //gp.~GnuplotPipe();
        gp.closePipe();
        //call destructor FIRST to close pipe an execute command 
        remove( sc_name.c_str() );
        std::cout<<"\n graph close";
    }
};




long double dybdx(long double x,long double y)
{
    //std::cout<<" "<<x<<" " <<y<<" ";
    return 2*x;
    //return cos(x);
    //return cos(x);
}
using namespace std;
template <class T>
T md (T a) {
  return (a<0?-a:a);
}
long double lin_ext(long double dydx(long double ,long double),long double x1,long double x2,long double y1,int t=10)
{
    long double y_tmp=y1,x_tmp=x1,delt=(x2-x1)/t;
    
    for(int i=0;i<t;i++)
    {
        //cout<<"e"<<x_tmp<<y_tmp;
        y_tmp=(dydx(x_tmp,y_tmp))*delt+y_tmp;//y=m(x2-x1)+y1=>y=dy+y1=>y2
        x_tmp+=delt;
    }
    
    
    //y_tmp=(dydx(x_tmp,y_tmp))*delt+y_tmp;
    return y_tmp;
}

long double opf(long double x)
{
    //return sin(x);
    return x*x;
}
int main(){
    
    gnugraph gg2("g1");
    gg2.show();
    gg2.show();
    
    cout<<"\n";
    
    
    
}
