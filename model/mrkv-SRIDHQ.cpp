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
double lin_ext(double dydx(double ,double),double x1,double x2,double y1,int t=10)
{
    double y_tmp=y1,x_tmp=x1,delt=(x2-x1)/t;
    
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
public:
    std::vector< std::vector<double> > populations;
    std::vector< double> time_vector;
    int vec_use;
    double del_step;
    //--------
    double transim;
    double recov;
    double birth_rate;
    double natural_death_rate;
    double infected_death_rate;
    double hospital_max;
    double hospital_admit_usual;
    double hospital_admit_infected;
    double hospital_recov;
    double hospital_death;
    double P_hosp_recov_usual;
    double P_hosp_recov_infected;
    
    //--------
    /*
    S I R 
    */
    model(std::vector<double> initiial_val)
    {
        //
        transim=2.2;
        recov=0.2;
        birth_rate=0.01;
        natural_death_rate=0.01;
        infected_death_rate=0.02;//2%
        hospital_max=0.1;//beds percent to population
        hospital_admit_usual=0.001;
        hospital_admit_infected=0.01;
        P_hosp_recov_usual=0.9;//90%
        P_hosp_recov_infected=0.8;//80%
        hospital_recov=0.5;
        hospital_death=0.1;
        //
        
        time_vector.insert(time_vector.end(),0);//t start=0
        std::vector<double> tmp;
        for (int i=0;i<initiial_val.size();i++)
        {
            tmp.clear();
            tmp.insert(tmp.end(),initiial_val[i]);
            populations.insert(populations.end(),tmp);
        }
        vec_use=initiial_val.size();
        //vectors initialized
        
        
        
        
    }
    double dSdt(double S,double I,double R,double Hin[],double Hout[],int j)
    {
        double outv;
        outv=-transim*S*I +birth_rate*S-natural_death_rate*S ;
        //--H--
        outv-=Hin[0];
        outv+=Hout[0]*P_hosp_recov_usual;//only recovered
        //--H--
        return outv;
    }
    double dIdt(double S,double I,double R,double Hin[],double Hout[],int j)
    {
        double outv;
        outv=transim*S*I-recov*I-infected_death_rate*I;
        //--H--
        outv-=Hin[1];
        outv+=Hout[1]*P_hosp_recov_infected;
        //--H--
        return outv;
    }
    double dRdt(double S,double I,double R,double Hin[],double Hout[],int j)
    {
        double outv;
        outv=recov*I +birth_rate*R-natural_death_rate*R;
        //--H--
        outv-=Hin[2];
        outv+=Hout[2]*P_hosp_recov_usual;
        //--H--
        return outv;
    }
    double dDdt(double S,double I,double R,double Hin[],double Hout[],int j)
    {
        double outv;
        //outv= +natural_death_rate*S+natural_death_rate*R+infected_death_rate*I;
        outv= infected_death_rate*I;
        outv+=Hout[0]*(1-P_hosp_recov_usual);
        outv+=Hout[1]*(1-P_hosp_recov_infected);
        outv+=Hout[2]*(1-P_hosp_recov_usual);
        
        return outv;
    }
    double dHdt(double Hin[],double Hout[],int j)
    {
        
        int hsp_stay_t=5;
        double outv=0;
        
        //{
        /*
        double beds_left;
        double S1;
        double I1;
        double R1;
        
            beds_left=(hospital_max-populations[4][j]);//h
            S1=hospital_admit_usual*populations[0][j];//S
            I1=hospital_admit_infected*populations[1][j];//I
            R1=hospital_admit_usual*populations[2][j];//R
            //IN
            if(S1+I1<beds_left)
            {
                outv+=S1+I1;
            }
            else if(beds_left<=0)
            {
                outv+=0;
            }
            else
            {
                outv+=del_step*beds_left*0.92;//beds only reach to 92%
            }
        
        //OUT
        if(j-hsp_stay_t>0)
        {
            beds_left=(hospital_max-populations[4][j-hsp_stay_t]);//h
            S1=hospital_admit_usual*populations[0][j-hsp_stay_t];//S
            I1=hospital_admit_infected*populations[1][j-hsp_stay_t];//I
            R1=hospital_admit_usual*populations[2][j-hsp_stay_t];//S
            //IN
            if(S1+I1+R1<beds_left)
            {
                outv-=(S1+I1+R1);
            }
            else if(beds_left<=0)
            {
                outv-=0;
            }
            else
            {
                outv-=del_step*beds_left*0.92;//beds only reach to 92%
            }
        }
        else
        {
            outv-=0;
        }
        */
        //}
        
        for (int i=0;i<3;i++)
        {
            outv+=Hin[i];
            outv-=Hout[i];
        }
        return outv;
    }
    
    
    void ODEsolve(double delt1,double t_st,int t_itr)
    {
        del_step=delt1;
        time_vector[0]=t_st;
        std::vector<double> tmp1,dvdt1;
        for(int i=0;i<vec_use;i++ )
        {
            dvdt1.insert(dvdt1.end(),0);
        }
        double transim_bk=transim;
        for (int i=0;i<t_itr;i++)
        {
            //--Q--
            //if(transim<transim_bk){transim=transim*0.001;}//quarentin
            if((time_vector[i]>=0.5)&&(time_vector[i]<10))
            {
                
                //std::cout<<"\n transim"<<transim;
                if(transim<transim_bk){transim+=transim*0.09;}//quarentin
                
                else{transim=0.2;}
            }
            else
            {
                transim=transim_bk;
                
            }
            //--Q--
            
            //--H--
            //{
            int hsp_stay_t=5;
            //double outv=0;
            double beds_left;
            double Hin[3]={0,0,0},Hout[3]={0,0,0};// 0 S, 1 I,2 R
            double S1,Sa=0,Sr=0;
            double I1,Ia=0,Ir=0;
            double R1,Ra=0,Rr=0;
            
            
            double beds_freed=0;
            beds_freed+=Hout[0]+Hout[1]+Hout[2];
                beds_left=(hospital_max-populations[4][i]);//h
                //beds_left+=beds_freed;
                S1=hospital_admit_usual*populations[0][i];//S
                I1=hospital_admit_infected*populations[1][i];//I
                R1=hospital_admit_usual*populations[2][i];//R
                //IN
                if(S1+I1+R1<beds_left)
                {
                    //outv+=S1+I1+R1;
                    Hin[0]+=S1;
                    Hin[1]+=I1;
                    Hin[2]+=R1;
                }
                else if(beds_left<=0)
                {
                    Hin[0]+=0;
                    Hin[1]+=0;
                    Hin[2]+=0;
                }
                else
                {
                    //outv+=del_step*beds_left*0.92;//beds only reach to 92%
                    double scale_val_b=(del_step*beds_left*0.92)/(S1+I1+R1);
                    Hin[0]+=S1*scale_val_b;
                    Hin[1]+=I1*scale_val_b;
                    Hin[2]+=R1*scale_val_b;
                }
            //OUT
            if(i-hsp_stay_t>0)
            {
                beds_left=(hospital_max-populations[4][i-hsp_stay_t]);//h
                S1=hospital_admit_usual*populations[0][i-hsp_stay_t];//S
                I1=hospital_admit_infected*populations[1][i-hsp_stay_t];//I
                R1=hospital_admit_usual*populations[2][i-hsp_stay_t];//S
                //IN
                if(S1+I1+R1<beds_left)
                {
                    //outv-=(S1+I1+R1);
                    Hout[0]+=S1;
                    Hout[1]+=I1;
                    Hout[2]+=R1;
                }
                else if(beds_left<=0)
                {
                    //outv-=0;
                    Hout[0]+=0;
                    Hout[1]+=0;
                    Hout[2]+=0;
                }
                else
                {
                    //outv-=del_step*beds_left*0.92;//beds only reach to 92%
                    double scale_val_b=(del_step*beds_left*0.92)/(S1+I1+R1);
                    Hout[0]+=S1*scale_val_b;
                    Hout[1]+=I1*scale_val_b;
                    Hout[2]+=R1*scale_val_b;
                }
            }
            else
            {
                //outv-=0
            }
            
            //}
            //--H--
            
            //dvdt1[0]=2*(vt1[i]);
            
            dvdt1[0]=dSdt(populations[0][i],populations[1][i],populations[2][i],Hin,Hout,i);
            dvdt1[1]=dIdt(populations[0][i],populations[1][i],populations[2][i],Hin,Hout,i);
            dvdt1[2]=dRdt(populations[0][i],populations[1][i],populations[2][i],Hin,Hout,i);
            
            dvdt1[3]=dDdt(populations[0][i],populations[1][i],populations[2][i],Hin,Hout,i);
            
            dvdt1[4]=dHdt(Hin,Hout,i);
            //setting dvdt vector
            
            
            lin_ext_vec(populations,dvdt1,delt1);
            
            time_vector.insert(time_vector.end(),time_vector[i]+delt1);
        }
    }
    
    ~model()
    {
        
    }
    
};

using namespace std;
int main()
{
    //std::vector< std::vector<double> > populations;
    
    std::vector<double> initiial_val;
    initiial_val.insert(initiial_val.end(),0.99);//S 0
    initiial_val.insert(initiial_val.end(),0.01);//I 1
    initiial_val.insert(initiial_val.end(),0.0);//R 2
    initiial_val.insert(initiial_val.end(),0.0);//D 3
    initiial_val.insert(initiial_val.end(),0.0);//H 4
    initiial_val.insert(initiial_val.end(),0.0);//Q 5
    
    //cout<<"\n";for(int i=0;i<initiial_val.size();i++){cout<<" "<<initiial_val[i];}
    //initiial_val.clear();
    //cout<<"\n";for(int i=0;i<initiial_val.size();i++){cout<<" "<<initiial_val[i];}
    std::vector<double> tmp;
    tmp.clear();
    /*
    for (int i=0;i<initiial_val.size();i++)
    {
        tmp.clear();
        tmp.insert(tmp.end(),initiial_val[i]);
        populations.insert(populations.end(),tmp);
        //initiial_val.insert(initiial_val.end(),i);
    }
    
    
    
    cout<<"\n";
    for (int i=0;i<populations.size();i++)
    {
        for(int j=0;j<populations[i].size();j++)
        {
            cout<<" "<<populations[i][j];
        }
        cout<<"\n";
    }
    */
    model m1(initiial_val);
    m1.ODEsolve(0.1,0,400);
    
    std::string dat_file="pts2.dat";
    gnuplotpipe gp;
    std::ofstream ptsout;
    ptsout.open(dat_file.c_str());
    
    
    gp.send_line("plot  \"pts2.dat\" using 1:2 smooth cspline title 'S', \\");
    gp.send_line("\"pts2.dat\" using 1:3 smooth cspline title 'I', \\");
    gp.send_line("\"pts2.dat\" using 1:4 smooth cspline title 'R', \\");
    gp.send_line("\"pts2.dat\" using 1:5 smooth cspline title 'D', \\");
    gp.send_line("\"pts2.dat\" using 1:6 smooth cspline title 'H' ");
    for (int j=0;j<m1.populations[0].size();j++)
    {
        ptsout<<m1.time_vector[j];
        for(int i=0;i<m1.populations.size();i++)
        {
            ptsout<<" "<<m1.populations[i][j];
        }
         ptsout<<"\n";
    }
    
    
    
    ptsout.close();
    
    std::cout<<"\n prgret";
    
}