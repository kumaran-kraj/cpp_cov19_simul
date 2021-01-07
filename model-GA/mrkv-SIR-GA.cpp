#include <sys/stat.h>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include<math.h>
#include<sstream>
#include <stdlib.h>
#include<time.h>

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
    //--------
    double transim;
    double recov;
    double birth_rate;
    double death_rate;
    double infected_death_rate;
    
    //--------
    /*
    S I R 
    */
    model(std::vector<double> initiial_val)
    {
        //
        transim=2.2;
        recov=0.23;
        birth_rate=0.01;
        death_rate=0.0095;
        infected_death_rate=0.02;
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
    double dSdt(double S,double I,double R)
    {
        double outv;
        outv=-transim*S*I+birth_rate*S-death_rate*S;
        return outv;
    }
    double dIdt(double S,double I,double R)
    {
        double outv;
        outv=transim*S*I-recov*I -infected_death_rate*I;
        return outv;
    }
    double dRdt(double S,double I,double R)
    {
        double outv;
        outv=recov*I +birth_rate*R-death_rate*R;
        return outv;
    }
    double dDdt(double S,double I,double R)
    {
        double outv;
        outv=infected_death_rate*I ;//+death_rate*S+death_rate*R;
        return outv;
    }
    
    void ODEsolve(double delt1,double t_st,int t_itr)
    {
        time_vector[0]=t_st;
        std::vector<double> tmp1,dvdt1;
        for(int i=0;i<vec_use;i++ )
        {
            dvdt1.insert(dvdt1.end(),0);
        }
        
        for (int i=0;i<t_itr;i++)
        {
            
            //dvdt1[0]=2*(vt1[i]);
            dvdt1[0]=dSdt(populations[0][i],populations[1][i],populations[2][i]);
            dvdt1[1]=dIdt(populations[0][i],populations[1][i],populations[2][i]);
            dvdt1[2]=dRdt(populations[0][i],populations[1][i],populations[2][i]);
            dvdt1[3]=dDdt(populations[0][i],populations[1][i],populations[2][i]);
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

struct fit_val
    {
        double fitv;
        int org_id;
    };
bool comp_fit(fit_val a,fit_val b)
    {
        return a.fitv<b.fitv;
    }
class GA
{
    
public:
    
    
    
    std::vector<double> inp_p;
    std::vector< std::vector<double> > organism;
    std::vector< std::vector<double> > LbUb;
    model *Actual;
    int num_params;
    int num_org;
    GA(std::vector< std::vector<double> > lbub,int num_pram=1,int number_oranisms=100)
    {
        num_params=num_pram;
        num_org=number_oranisms;
        
        for(int i=0;i<num_params;i++){inp_p.insert(inp_p.end(),0);}
        LbUb.insert(LbUb.end(),lbub[0]);//lb
        LbUb.insert(LbUb.end(),lbub[1]);//ub
        for(int i=0;i<num_org;i++)
        {
            
            //std::cout<<"\n";
            for(int j=0;j<num_params;j++)
            {
                inp_p[j]=rand_rg(lbub[0][j],lbub[1][j]);
                //LbUb[i][j]=lbub[i][j];
                //std::cout<<inp_p[j]<<" ";
            }
            organism.insert(organism.end(),inp_p);
        }
        
        //{val gen
        double initial_split=0.01;//%infected
        std::vector<double> initiial_val;
        initiial_val.insert(initiial_val.end(),1-initial_split);//S
        initiial_val.insert(initiial_val.end(),initial_split);//I
        initiial_val.insert(initiial_val.end(),0.0);//R
        initiial_val.insert(initiial_val.end(),0.0);//D
        Actual =new model(initiial_val);
        double t_del=0.4;
        Actual->ODEsolve(t_del,0,100);
        //val gen}
        
        double t_acc=0;
        std::vector<double>t_val;std::vector< std::vector<double> >tmp;
        
        for(int i=0;i<100;i++)
        {t_val.insert(t_val.end(),t_acc);t_acc+=t_del;}
        //{test show
        /*
        cplot gts("dat",true,true,true);
        tmp.insert(tmp.end(),t_val);
        gts.addarr(tmp);
        gts.addarr(Actual->populations);
        gts.add_line("set datafile separator \",\"");
        gts.add_line("plot  \""+gts.dat_file+"\" using 1:2 smooth cspline title 'S', \\");
        gts.add_line("\""+gts.dat_file+"\" using 1:3 smooth cspline title 'I', \\");
        gts.add_line("\""+gts.dat_file+"\" using 1:4 smooth cspline title 'R', \\");
        gts.add_line("\""+gts.dat_file+"\" using 1:5 smooth cspline title 'D', \\");
        gts.show();
        //std::cin.ignore();
        */
        //}test show
        
        
    }
    double rand_rg(double lb,double ub)
    {
        int v=rand();
        v=v%1000;
        //cout<<"\nv:"<<v;
        //vd is from 0.0 to 0.999
        double wd=ub-lb,vd=v/1000.0;
        //cout<<"\nv:"<<v<<" vd"<<vd;
        vd=vd*wd;
        vd=vd+lb;
        return vd;
    }
    double inline errf(double x,double y)
    {
        return sqrt(pow((x-y),2));
    }
    double simul(std::vector<double> params,std::string gorg="pts",bool show_g=false)
    {
        //{
        //std::vector< std::vector<double> > populations;
        double initial_split=params[3];//%infected
        std::vector<double> initiial_val;
        initiial_val.insert(initiial_val.end(),1-initial_split);//S
        initiial_val.insert(initiial_val.end(),initial_split);//I
        initiial_val.insert(initiial_val.end(),0.0);//R
        initiial_val.insert(initiial_val.end(),0.0);//D
        
        model m1(initiial_val);
        m1.transim=params[0];
        m1.recov=params[1];
       // m1.birth_rate=params[2];
        //m1.death_rate=params[3];
        m1.infected_death_rate=params[2];
        
        
        double t_del=0.4;
        m1.ODEsolve(t_del,0,100);
        
        double err_acc=0;
        for (int i=0;i<m1.populations.size();i++ )
        {
            for (int j=0;j<m1.populations[i].size();j++)
            {
                err_acc+=errf(m1.populations[i][j],Actual->populations[i][j]);
                //cout<<"\n "<<errf(m1.populations[i][j],Actual->populations[i][j]);
            }
            //cout<<"\n "<<err_acc;
        }
        
        std::string dat_file=gorg+".dat";
        gnuplotpipe gp;
        ///*//test show
        if (show_g)
        {
            std::ofstream ptsout;
            
            ptsout.open(dat_file.c_str());
            
            gp.send_line("set terminal svg dashlength 0.2");
            gp.send_line("set output \""+gorg+".svg\"");
            gp.send_line("set terminal svg dashlength 0.2");
            gp.send_line("plot  \""+dat_file+"\" using 1:2 smooth cspline title 'S', \\");
            gp.send_line("\""+dat_file+"\" using 1:3 smooth cspline title 'I', \\");
            gp.send_line("\""+dat_file+"\" using 1:4 smooth cspline title 'R', \\");
            gp.send_line("\""+dat_file+"\" using 1:5 smooth cspline title 'D', \\");
            int jdelay=0;
            
            ptsout<<"#params";
            for(int i=0;i<params.size();i++)
            {ptsout<<" "<<params[i];}
            ptsout<<" errf"<<err_acc;
            ptsout<<"\n";
            
            for (int j=jdelay;j<m1.populations[0].size();j++)
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
        //*/
        
        
        
        
        //}
        
        return err_acc;
    }
    void generation_run(int iters)
    {
        //std::cout<<"\n "<<organism.size();
        fit_val f_ar[num_org];
        int pcnt=num_org/10;
        int prev_b,num_tries=0;
        for(int gni=0;gni<iters;gni++)
        {
            
            std::cout<<"\n gen:"<<gni<<" ";
            for(int ogi=0;ogi<num_org;ogi++)
            {
                
                //std::cout<<"\n org:"<<ogi<<" fitness"<<simul(organism[ogi]);
                f_ar[ogi].fitv=simul(organism[ogi]);
                //std::cout<<"\n org:"<<ogi<<" fitness"<<f_ar[ogi].fitv;
                f_ar[ogi].org_id=ogi;
                
                if(ogi%pcnt==0){cout<<"#";}
            }
            sort (f_ar,f_ar+num_org,comp_fit);
            cout<<"\n "<<" b " <<f_ar[0].fitv<<" i " <<f_ar[0].org_id;
            simul(organism[f_ar[0].org_id],"G"+SSTR(gni)+"R1",true);
            int i_cur,i_del,rn;
            for(int i=0;i<num_org/2;i++)
            {
                i_cur=f_ar[i].org_id;//upper half
                i_del=f_ar[i+num_org/2].org_id;//lower half
                rn=rand()%num_params;
                for(int j=0;j<num_params;j++)
                {
                    organism[i_del][j]=organism[i_cur][j];
                }
                organism[i_del][rn]=rand_rg(LbUb[0][rn],LbUb[1][rn]);
                //cout<<"\n ch"<<organism[i_del][rn];
                
            }
            if(gni!=0)
            {
                
                if(prev_b==f_ar[0].org_id)
                {
                    num_tries++;
                    std::cout<<"\n no change , retry "<<num_tries;
                    gni--;
                }
                else
                {
                    num_tries=0;//reset
                    prev_b=f_ar[0].org_id;
                }
                if(num_tries>=4)
                {
                    for(int i=0;i<num_org/2;i++)
                    {
                        i_cur=f_ar[i].org_id;//upper half
                        i_del=f_ar[i+num_org/2].org_id;//lower half
                        rn=rand()%num_params;
                        for(int j=0;j<num_params;j++)
                        {
                            organism[i_del][j]=organism[i_cur][j];
                        }
                        organism[i_del][rn]=rand_rg(LbUb[0][rn],LbUb[1][rn]);
                        //cout<<"\n ch"<<organism[i_del][rn];
                        
                    }
                }
                if(num_tries>=7)
                {
                    num_tries=0;
                    prev_b=f_ar[1].org_id;
                }
            }
            else
            {
                prev_b=f_ar[0].org_id;
            }
        }
    }
    
    ~GA()
    {
        
        
    }
};
int main()
{
    srand(time(0));
    
    std::vector<double> inp_p,tmpi;
    inp_p.insert(inp_p.end(),2.2);//spread
    inp_p.insert(inp_p.end(),0.23);//recov
    inp_p.insert(inp_p.end(),0.02);//inf_d_r
    inp_p.insert(inp_p.end(),0.01);//initial split infected
    //GA run1(4);
    std::vector< std::vector<double> > lbub;
    tmpi.insert(tmpi.end(),0.0);
    tmpi.insert(tmpi.end(),0.0);
    tmpi.insert(tmpi.end(),0.0);
    tmpi.insert(tmpi.end(),0.0);
    lbub.insert(lbub.end(),tmpi);
    tmpi[0]=4.0;
    tmpi[1]=0.5;
    tmpi[2]=0.5;
    tmpi[3]=0.05;
    lbub.insert(lbub.end(),tmpi);
    
    GA run1(lbub,4,100);
    std::cout<<"\n R "<<run1.simul(inp_p);
    run1.generation_run(12);
    
    
    
    
    
}