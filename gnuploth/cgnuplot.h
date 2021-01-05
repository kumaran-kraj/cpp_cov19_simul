#pragma once
#include <sys/stat.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include<math.h>
#include<sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
        
//using namespace std;
inline bool file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

class gnuplotpipe 
{
private:
    FILE* pipe;
    //std::vector<std::string> buffer;
    //std::string graph_name;
    std::ofstream ptsout;
public:
    //std::string dat_file="graph1.dat";
    gnuplotpipe( bool start_pipe=true)
    {
        //std::cout << "\n pipecst";
        pipe=NULL;
        //graph_name=gname;
        //dat_file=graph_name+".dat";
        if(start_pipe)
        {pipe_open();}
    }
    void pipe_open(bool persist=true)
    {
        if (!pipe)
        {
            //std::cout << "\n pipeopen";
            pipe = popen(persist ? "gnuplot -persist" : "gnuplot", "w");
        }
        
    }
    void pipe_close()
    {
        if (pipe)
        {
            //std::cout << "\n pipeclose";
            pclose(pipe);
            pipe=NULL;
        }
        
    }
    void send_line(std::string text)
    {
        //std::cout<<"\n sendline";
        if (!pipe)
        {
            std::cout<<" -no pipe ";
            return;
        }
        fputs((text + "\n").c_str(), pipe);
    }
    ~gnuplotpipe()
    {
        
        pipe_close();
        //std::cout << "\n pipedst";
    }
};


class cplot
{
    gnuplotpipe *g1;
    //std::string g_name;
    
    std::string sc_name;
    
    std::string script_Q;
    std::ofstream plot_script;
    std::ofstream point_dat;
    bool save_op;
public:  
    std::string dat_file;
    cplot(std::string datafile_name="dat",bool save_output=false,bool overwrite=false)
    {
        save_op=save_output;
        //std::cout << "\n plotcst";
        g1=new gnuplotpipe("name");
        //g1->pipe_open();
        //g1->pipe_close();
        //g1->dat_file="e";
        
        std::string b_name=datafile_name,r_cd=datafile_name;
        if(!overwrite)
        {
            for (int i=0;i<1024;i++)
            {
                r_cd=b_name+SSTR(i);
                //cout<<"\n "<<r_cd;
                if(!file_exists(r_cd+".dat"))
                {
                    
                    //std::cout<<"\n "<<r_cd;
                    break;
                }
            }
        }
        
        sc_name=r_cd+".p";
        dat_file=r_cd+".dat";
        
        point_dat.open(dat_file.c_str());
        point_dat<<"#\n";
        point_dat.close();
        
        plot_script.open(sc_name.c_str());
        plot_script<<"#\n";
        plot_script.close();
        
        
        
    }
    void add_line(std::string enq)
    {
        script_Q.append(enq+"\n");
    }
    void add_data(const std::string &dat1)
    {
        point_dat.open(dat_file.c_str());
        point_dat<<dat1;
        point_dat.close();
    }
    void show (std::string f_name="\\?.deafult:class assigned file.?\\")
    {
        //write script Q
        plot_script.open(sc_name.c_str(),std::ios::out | std::ios::app);
        plot_script<<script_Q;
        plot_script.close();
        //write points array
        
        
        g1->pipe_open();
        if(f_name=="\\?.deafult:class assigned file.?\\")
        {g1->send_line("load \""+sc_name+"\"");}
        else
        {
            g1->send_line("load \""+f_name+"\"");
        }
        //std::cout<<"\n plotting";
        g1->pipe_close();
    }
    ~cplot()
    {
        
        delete g1;
        if(!save_op)
        {
            remove( sc_name.c_str() );
            remove( dat_file.c_str() );
        }    
        
        //std::cout << "\n plotdst";
    }
};