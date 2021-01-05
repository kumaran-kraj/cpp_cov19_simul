#include <sys/stat.h>
#include <algorithm>
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
    
    //std::string script_Q;
    std::fstream plot_script;
    std::fstream point_dat;
    
    bool save_op;
public:  
    std::string dat_file;
    //std::vector<std::string> plot_Q;
    char delim;
    cplot(std::string datafile_name="dat",bool use_given=false,bool save_output=false,bool overwrite=false)
    {
        delim=',';
        save_op=save_output;
        //std::cout << "\n plotcst";
        g1=new gnuplotpipe("name");
        //g1->pipe_open();
        //g1->pipe_close();
        //g1->dat_file="e";
        
        std::string b_name=datafile_name,r_cd=datafile_name;
        if(!use_given)
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
        
        if(overwrite||!use_given)
        {
            point_dat.open(dat_file.c_str(),std::ios::out);
            point_dat<<"#\n";
            point_dat.close();
            
            plot_script.open(sc_name.c_str(),std::ios::out);
            plot_script<<"#\n";
            plot_script.close();
        }
        
        
    }
    void add_line(std::string enq)
    {
        //script_Q.append(enq+"\n");
        plot_script.open(sc_name.c_str(),std::ios::out | std::ios::app);
        plot_script<<enq<<"\n";
        plot_script.close();
    }
    /*
    void add_data(const std::string &dat1)
    {
        
    }
    */
    template<typename T>
    void addarr( std::vector<std::vector<T> > pts)
    {
        std::vector<int> szs;
        int mxs=0,r_ctr=0,ctx=0;
        for (int i=0;i<pts.size();i++)
        {
            szs.push_back(pts[i].size());
            //std::cout<<"\n "<<szs[i];
        }
        mxs=*std::max_element(szs.begin(),szs.end());
        //std::cout<<"\n max"<<mxs;
        r_ctr=mxs;
        
        
        std::string tmp_n=dat_file.substr(0,dat_file.size()-4)+".tmp";//temp file
        std::fstream tmp_file;
        //std::cout<<"\n "<<dat_file;
        //tmp_n="tmp.csv";
        tmp_file.open(tmp_n.c_str(),std::ios::out);
        point_dat.open(dat_file.c_str(),std::ios::in);//"tst.csv"
        //point_dat.open("tst.csv",std::ios::in);//
        
            
        std::string intmp;
        std::string line1="";
        int c=0,f1=0;
        
        if(getline(point_dat,intmp))
        {
            
            if(intmp[0]=='#')// 1 first line exists
            {
                //std::cout<<"\n check L1 intmp:"<<intmp;
                line1=intmp;
                f1=1;
            }
            else// -1 no first line
            {
                f1=-1;
                point_dat.seekg(0, std::ios::beg);
            }
        }
        
        if(getline(point_dat,intmp))
        {
            c = std::count(intmp.begin(),intmp.end(),',');
            if(intmp!=""){c++;}
        }
        
        point_dat.seekg(0, std::ios::beg);
        if(f1==1)
        {
            getline(point_dat,intmp);
        }
        
        if(line1!="")
        {
            tmp_file<<line1<<"\n";
        }
        
        //std::cout<<"\n----------------------";
        while(getline(point_dat,intmp))
        {
            //std::cout<<"\n "<<intmp;
            tmp_file<<intmp;
            for (int i=0;i<pts.size();i++)
            {
                tmp_file<<delim;
                if(ctx<pts[i].size())
                {
                    tmp_file<<pts[i][ctx];
                }
            }
            ctx++;
                
                
                
            tmp_file<<"\n";    
        }
        while (ctx<mxs)
        {
            for(int i=0;i<c-1;i++)
            {
                tmp_file<<delim;
            }
            for (int i=0;i<pts.size();i++)
            {
                if(i!=0||c!=0)
                {tmp_file<<delim;}//ignore first ',' if new file 
                if(ctx<pts[i].size())
                {
                    tmp_file<<pts[i][ctx];
                }
            }
            ctx++;
                
                
                
            tmp_file<<"\n";
        }
        
        
        //std::cout<<"\n----------------------";
        //std::cout<<"\n line1:"<<line1<<" ||exisis:"<<f1;
        //std::cout<<"\n "<<" entries:"<<c;
        
        tmp_file.close();
        point_dat.close();
        
        remove(dat_file.c_str());
        rename(tmp_n.c_str(),dat_file.c_str()); 
        
        
    }
    
    void show (std::string f_name="\\?.deafult:class assigned file.?\\")
    {
        //write script Q
        /*
        plot_script.open(sc_name.c_str(),std::ios::out | std::ios::app);
        plot_script<<script_Q;
        plot_script.close();
        */
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
using namespace std;
int main()
{
    /*
    cplot ct("dat",false,true),ct2,ct3,ct4,ct5;
    
    ct.add_line("set title \"Energy\" ");
    //ct.show();
    ct.add_line("plot x, \""+ct.dat_file+"\" using 1:2 smooth cspline title 'fx'");
    // ct.show();
    std::string s11="1.4000 4.7547 4.7546 -0.0001 4.7469 -0.0078 4.7488 -0.0059 4.7469 -0.0078 4.7417 -0.0130\n1.5000 5.7029 5.7027 -0.0002 5.6919 -0.0111 5.6945 -0.0084 5.6919 -0.0111 5.6849 -0.0180\n1.6000 6.7980 6.7977 -0.0003 6.7831 -0.0149 6.7866 -0.0114 6.7831 -0.0149 6.7741 -0.0239\n1.7000 8.0604 8.0599 -0.0005 8.0408 -0.0196 8.0453 -0.0151 8.0408 -0.0196 8.0295 -0.0309\n1.8000 9.5131 9.5124 -0.0007 9.4879 -0.0252 9.4936 -0.0194 9.4879 -0.0252 9.4740 -0.0391";
    ct. add_data(s11);
    ct.show();
    ct2.show("sc.p");
    
    gnuplotpipe gt1;
    gt1.send_line("plot x**2 -x\n");
    gt1.pipe_close();
    gt1.send_line("plot x**3 -x\n");
    gt1.pipe_open();
    gt1.send_line("plot x*2 -x\n");
    
    
    std::string b_name="dat",r_cd;
    for (int i=0;i<10;i++)
    {
        r_cd=b_name+SSTR(i)+".dat";
        cout<<"\n "<<r_cd;
        cout<<"\n "<<file_exists(r_cd);
    }
    */
    //std::string dat_file="tst.csv";
    //std::fstream point_dat;
    /*
    point_dat.open(dat_file.c_str(),std::ios::in);
    
    std::string intmp;
    
    std::string line1="";
    int c=0,f1=0;
    
    if(getline(point_dat,intmp))
    {
        //point_dat>>intmp;
        // 1 first line exists
        if(intmp[0]='#')
        {
            line1=intmp;
            f1=1;
        }
        // -1 no first line 
        else
        {
            f1=-1;
            point_dat.seekg(0, std::ios::beg);
        }
    }
    
    
    if(getline(point_dat,intmp))
    {
        //point_dat>>intmp;
        c = std::count(intmp.begin(),intmp.end(),',');
        if(intmp!=""){c++;}
    }
    
    point_dat.seekg(0, std::ios::beg);
    if(f1==1)
    {
        getline(point_dat,intmp);
    }
    
    std::cout<<"\n----------------------";
    while(getline(point_dat,intmp))
    {
        
        //point_dat>>intmp;
        std::cout<<"\n "<<intmp;
    }
    std::cout<<"\n----------------------";
    std::cout<<"\n line1:"<<line1<<" ||exisis:"<<f1;
    //point_dat>>intmp;
    
    //c = std::count(intmp.begin(),intmp.end(),',');
    //if(intmp!=""){c++;}
    std::cout<<"\n "<<" entries:"<<c;
    point_dat.close();
    */
    /*
    std::vector< std::vector<int> > pts;
    int n=3;
    
    for(int i=0;i<n;i++)
    {
        std::vector<int> v1;
        for(int j=0;j<n;j++)
        {
            v1.insert(v1.end(),i+j);
        }
        pts.insert(pts.end(),v1);
    }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            std::cout<<" "<<pts[i][j];
        }
        std::cout<<"\n";
    }
    for(int i=0;i<n;i++)
    {
        pts[i].insert(pts[i].end(),i);
    }
    
    
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<=n;j++)
        {
            std::cout<<" "<<pts[i][j];
        }
        std::cout<<"\n";
    }
    
    
    
    cplot ct("dat",true,true,true);
    //std::cin.ignore();
    ct.addarr(pts);
    //std::cin.ignore();
    
    ct.addarr(pts);
    //std::cin.ignore();
    
    ct.addarr(pts);
    //std::cin.ignore();
    std::cout<<"\n dfcl:"<<ct.dat_file;
    ct.add_line("set datafile separator \",\"");
    ct.add_line("plot \""+ct.dat_file+"\" using 1:2 smooth cspline");
    ct.show();
    */
    
    
    
    std::cout<<"\n prgret";
    
}