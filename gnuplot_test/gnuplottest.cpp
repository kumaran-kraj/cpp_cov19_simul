//#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include<math.h>

class GnuplotPipe {
    //std::string dat_file="pts.dat";
    std::string g_name;
    std::ofstream ptsout;
public:
    std::string dat_file="pts.dat";
     GnuplotPipe(std::string graphName="Graph1",bool persist = true) {
        
        dat_file=graphName+".dat";
        g_name=graphName;
        
        std::cout << "Opening gnuplot... ";
        pipe = popen(persist ? "gnuplot -persist" : "gnuplot", "w");
        
        if (!pipe)
            std::cout << "pipe open failed!" << std::endl;
        else
            std::cout << "pipe open succeded." << std::endl;
        
        ptsout.open(dat_file.c_str());
        ptsout<<"#\n";
        ptsout.close();
    }// constructor creates pipe
    inline virtual ~GnuplotPipe(){
        if (pipe) pclose(pipe);
        remove( dat_file.c_str() );
        
    }// destructor closes pipe , easy to work with as scope takes care of pipe deletion

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
private:
    GnuplotPipe(GnuplotPipe const&) = delete;
    void operator=(GnuplotPipe const&) = delete;

    FILE* pipe;
    std::vector<std::string> buffer;
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

void lin_ext_vec(std::vector< std::vector<double> > &val1,std::vector<double> dvdt,double dlt=1)
{
    //long double y_tmp=y1,x_tmp=x1,delt=dlt/t;
    //std::vector<long double> tmp_v;
    /*
    for(int i=0;i<t;i++)
    {
        //cout<<"e"<<x_tmp<<y_tmp;
        cout<<"\n";
        for(int i=0;i<tmp_v.size();i++)
            {cout<<" "<<tmp_v[i];}
        //y_tmp=(dydx(x_tmp,y_tmp))*delt+y_tmp;//y=m(x2-x1)+y1=>y=dy+y1=>y2
        tmp_v[ext_pos]=dydx(tmp_v);
        //x_tmp+=delt;
    }
    */
    double v_new;
    for (int i=0;i<val1.size();i++)
    {
        v_new=val1[i][val1[i].size()-1] + dvdt[i]*dlt;
        val1[i].insert(val1[i].end(),v_new);
    }
    
    
    
    //y_tmp=(dydx(x_tmp,y_tmp))*delt+y_tmp;
    //return tmp_v[ext_pos];
}

class model
{
   //y=(dy/dx)*(x-x1)+y1
public:
    //Succeptable , Infected , Recovered , Diseased
    vector<vector<long double> > p_states;
    model()
    {
        p_states [0][0]=10000;//total current population
        p_states [1][0]=0;//infected 
        p_states [2][0]=0;//recovered
        p_states [2][0]=0;//diseased
    }
    
    ~model()
    {
        
    }
    
};

class scpt
{
public:
    int scl;
    scpt()
    {
        scl=10;
        
    }
    long double dydx (std::vector<long double> inp)
    {
        
        return cos(inp[0]);
    }
    ~scpt()
    {
        
        
    }
    
};



long double opf(long double x)
{
    //return sin(x);
    return x*x;
}
int main(){
    GnuplotPipe gp;
    GnuplotPipe gp1("gtst");
    double x1=0,y1=0,x2,y2,delt=0.1;
    x2=x1;y2=y1;
    string dat_file="pts.dat";
    
    std::ofstream ptsout;
    ptsout.open(dat_file.c_str());
    
    
    std::vector<long double> pt_op ;
    pt_op.push_back(0);pt_op.push_back(0);pt_op.push_back(0);
    /*
   //cout<<x2<<" "<<y2<<" "<<opf(x2)<<"\n";
   ptsout<<x2<<" "<<y2<<" "<<opf(x2)<<"\n";
    for(int i =0;i<200;i++)
    {
        x2=x1+delt;
        y2=lin_ext(dybdx,x1,x2,y1,50);
        
        pt_op[0]=x2;
        pt_op[1]=y2;
        pt_op[2]=opf(x2);
        gp1.writePoints(pt_op);
        
        //cout<<x2<<" "<<y2<<" "<<opf(x2)<<"\n";
        ptsout<<x2<<" "<<y2<<" "<<opf(x2)<<"\n";
        y1=y2;
        x1=x2;
    }
    */
    
    
    
    
    //gp.sendLine("plot [-pi/2:pi] cos(x),-(sin(x) > sin(x+1) ? sin(x) : sin(x+1))");
    
    gp.sendLine("plot  \"pts.dat\" using 1:2 smooth cspline title 'fx', \\");
    gp.sendLine("\"pts.dat\" using 1:3 smooth cspline title 'linextvec'");
    
    //gp1.sendLine("plot  \"pts.dat\" using 1:2 smooth cspline title 'f(x)', \\");
    //gp1.sendLine("\"pts.dat\" using 1:3 smooth cspline title 'lin_exp'");
    
    double delt1=1;
    std::vector<double> tmp1,dvdt1,vt1;
    std::vector< std::vector<double> > val1;
    tmp1.insert(tmp1.end(),0);
    val1.insert(val1.end(),tmp1);//initial val only one variable
    vt1.insert(vt1.end(),0);//time step start
    dvdt1.insert(dvdt1.end(),0);
    for (int i=0;i<1000;i++)
    {
        
        dvdt1[0]=2*(vt1[i]);
        
        //setting dvdt vector
        lin_ext_vec(val1,dvdt1,delt1);
        vt1.insert(vt1.end(),vt1[i]+delt1);
    }
    
    for (int j=0;j<val1[0].size();j++)
    {
        ptsout<<vt1[j]<<" "<<vt1[j]*vt1[j];
        for(int i=0;i<val1.size();i++)
        {
            ptsout<<" "<<val1[i][j];
        }
         ptsout<<"\n";
    }
    
    
    
    ptsout.close();
    /*
    GnuplotPipe gp1;
    gp1.sendLine("plot [-pi/2:pi] cos(x),-(sin(x) > sin(x+1) ? sin(x) : sin(x+1))");
    */
    
    return 0;
}
