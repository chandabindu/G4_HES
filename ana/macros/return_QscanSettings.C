void return_QscanSettings(){

  int bin_fg = 21;
  double min_fg = -2.0;
  double max_fg =  2.0;
  double bin_wd = (max_fg-min_fg)/(bin_fg-1);
  double Q1;
  double Q2;
	//cout<<"Q1 Scale Factor: ";cin>>Q1;
	//cout<<"Q2 Scale Factor: ";cin>>Q2;
	//double Q1set = (Q1-min_fg)/bin_wd;
	//double Q2set = (Q2-min_fg)/bin_wd;
	//int SettingNum = bin_fg*Q1set+Q2set;
  

	//cout<<"Q1 set: "<<Q1set<<endl;
	//cout<<"Q2 set: "<<Q2set<<endl;
    //cout<<"Setting Number: "<< SettingNum<<endl;
	int best_setup=309;
	double fg_q1 =  min_fg + ((max_fg-min_fg)/(bin_fg-1))*(int (best_setup/bin_fg));
	double fg_q2 =  min_fg + ((max_fg-min_fg)/(bin_fg-1))*(int (best_setup%bin_fg));
	cout<<"Q1 set: "<<fg_q1<<endl;
	cout<<"Q2 set: "<<fg_q2<<endl;

//return SettingNum;
}
