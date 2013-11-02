#include "config.h"
#include "tools.h"

CConfig::CConfig()
{	
	debug = 0;
	listen_port = 0;
	site_ip = "";
	connect_ip = "";
	site_port = 0;
	listen_interface = "";
	listen_ip = "";
	server_string  = "";
	fake_server_string = 0;
	trytorelink = 0;
	bounce_data_con = 0;	
	use_ident = 0;
	enforce_ident = 0;
	enforce_tls = 0;
	enforce_tls_fxp = 0;	
	send_traffic_info = 0;
	relink_ip = "";
	relink_port = 0;
	relink_user = "";
	relink_pass = "";
	user_access_denied = "";
	user_login_success = "";
	add_to_passive_port = 0;	
	port_range_start = 0;
	port_range_end = 0;
	use_port_range = 0;	
	buffersize = 0;

	pending = 0;
	admin_list = "";
	connect_timeout = 0;
	ident_timeout = 0;
	read_write_timeout = 0;

	cert_path = "";
	fxp_fromsite_list = "";
	use_fxpfromsite_list = 0;
	use_fxptosite_list = 0;
	fxp_tosite_list = "";
	uid = 0;
	debug_logfile = "";
	log_to_screen = 0;
	thread_limit = 0;
	ssl_ascii_cache = 0;
	use_ssl_exclude = 0;
	sslexclude_list = "";

	entry_list = "";
	infocmd = "";
	helpcmd = "";
	admincmd = "";
	tositecmd = "";
	fromsitecmd = "";
	sslexcludecmd = "";
	site_closed = "";
	site_full = "";
	usecommands = 0;
	showconnectfailmsg = 0;
	pidfile = "";
	connectfailmsg = "";
}

CConfig::~CConfig()
{
}

string CConfig::getkey(string name,string data)
{
	string value = "ERROR";
	unsigned int start,end;
	string tmp = data;
	start = tmp.find(name,0);
	if (start == string::npos)
	{
		for (unsigned int i=0;i<data.length();i++) { data[i] = '0'; }
		return value;
	}
	end = tmp.find(";",start);
	if (end == string::npos)
	{
		for (unsigned int i=0;i<data.length();i++) { data[i] = '0'; }
		return value;
	}
	value = tmp.substr(start+1 + name.length(),end-start-name.length()-1);
	for (unsigned int i=0;i<data.length();i++) { data[i] = '0'; }
	return value;
}

int CConfig::readconf(string filename,string key)
{
	ifstream conffile(filename.c_str(), ios::binary | ios::in);
	if (!conffile)
	{
		cout << "Could not find config file!\n";
		return 0;
	}
	else
	{
		
		int start,end,outlen;
		start = conffile.tellg();
		conffile.seekg(0,ios::end);
		end = conffile.tellg();
		conffile.seekg(0,ios::beg);
		unsigned char *bufferin,*bufferout;
		bufferin = new unsigned char [(end-start)  ];
		bufferout = new unsigned char [(end-start) ];
		memset(bufferin,0,end-start);
		memset(bufferout,0,end-start );
		conffile.read((char*)bufferin,end-start);
		conffile.close();
		unsigned char ivec[8];
		memset(ivec,0, sizeof(ivec));
		int ipos = 0;
		outlen = end-start;
		string daten;
		
		if (use_blowconf == 1)
		{
			
			EVP_CIPHER_CTX ctx;
			EVP_CIPHER_CTX_init(&ctx);
	    EVP_CipherInit_ex(&ctx, EVP_bf_cfb(), NULL, NULL, NULL,ipos );
	    EVP_CIPHER_CTX_set_key_length(&ctx, key.length());
	    EVP_CipherInit_ex(&ctx, NULL, NULL,(unsigned char*)key.c_str(), ivec,ipos );
	
			if(!EVP_CipherUpdate(&ctx, bufferout, &outlen, bufferin, end-start))
			{
				delete [] bufferin;
	 			delete [] bufferout;
				return 0;
			}
			daten = ((const char*)bufferout);
	    memset(bufferin,0,end-start);
			memset(bufferout,0,end-start );
	    delete [] bufferin;
	 		delete [] bufferout;
	 		
	 		EVP_CIPHER_CTX_cleanup(&ctx);
		}
		else
		{			
			daten = ((const char*)bufferin);
	    memset(bufferin,0,end-start);
			memset(bufferout,0,end-start );
	    delete [] bufferin;
	 		delete [] bufferout;
		}    
		
 		
 		
   	string val;

		if ((val=getkey("listen_port",daten)) != "ERROR")
   	{
   		listen_port = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "wrong key or listen_port missing\n";
   		
   		return 0;
   	}

   	if ((val=getkey("debug",daten)) != "ERROR")
   	{
   		debug = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "debug missing\n";
   		
   		return 0;
   	}
   	
		
		if ((val=getkey("use_fxpfromsite_list",daten)) != "ERROR")
    {
   		use_fxpfromsite_list = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "use_fxpfromsite_list missing\n";
   		
   		return 0;
   	}
		
   	if ((val=getkey("site_port",daten)) != "ERROR")
   	{
   		site_port = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "site_port missing\n";
   		
   		return 0;
   	}

   	if ((val=getkey("site_ip",daten)) != "ERROR")
   	{
   		site_ip = val;
   	}
   	else
   	{
   		cout << "site_ip missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("entry_list",daten)) != "ERROR")
   	{
   		entry_list = val;
   	}
   	else
   	{
   		cout << "entry_list missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("connect_ip",daten)) != "ERROR")
   	{
   		connect_ip = val;
   	}
   	else
   	{
   		cout << "connect_ip missing\n";
   		
   		return 0;
   	}
		
   	if ((val=getkey("cert_path",daten)) != "ERROR")
   	{
   		cert_path = val;
   	}
   	else
   	{
   		cout << "cert_path missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("fxp_fromsite_list",daten)) != "ERROR")
    {
   		fxp_fromsite_list = val;
   	}
   	else
   	{
   		cout << "fxp_fromsite_list missing\n";
   		
   		return 0;
   	}
		
 		if ((val=getkey("listen_interface",daten)) != "ERROR")
    {
   		listen_interface = val;
   	}
   	else
   	{
   		cout << "listen_interface missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("listen_ip",daten)) != "ERROR")
    {
   		listen_ip = val;
   	}
   	else
   	{
   		cout << "listen_ip missing\n";
   		
   		return 0;
   	}
		
		
 		if ((val=getkey("server_string",daten)) != "ERROR")
    {
   		server_string = val;
   	}
   	else
   	{
   		cout << "server_string missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("fake_server_string",daten)) != "ERROR")
    {
   		fake_server_string = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "fake_server missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("trytorelink",daten)) != "ERROR")
    {
   		trytorelink = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "trytorelink missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("bounce_data_con",daten)) != "ERROR")
    {
   		bounce_data_con = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "bounce_data_con missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("enforce_tls_fxp",daten)) != "ERROR")
    {
   		enforce_tls_fxp = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "enforce_tls_fxp missing\n";
   		
   		return 0;
   	}


 		if ((val=getkey("use_ident",daten)) != "ERROR")
    {
   		use_ident = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "use_ident missing\n";
   		
   		return 0;
   	}

		if ((val=getkey("enforce_ident",daten)) != "ERROR")
   	{
   		enforce_ident = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "enforce_ident missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("enforce_tls",daten)) != "ERROR")
    {
	 		enforce_tls = atoi(val.c_str());
	 	}
	 	else
	 	{
	 		cout << "enforce_tls missing\n";
	 		
	 		return 0;
	 	}

 		if ((val=getkey("send_traffic_info",daten)) != "ERROR")
    {
   		send_traffic_info = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "send_traffic_info missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("relink_ip",daten)) != "ERROR")
   	{
   		relink_ip = val;
   	}
   	else
   	{
   		cout << "relink_ip missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("relink_port",daten)) != "ERROR")
   	{
   		relink_port = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "relink_port missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("relink_user",daten)) != "ERROR")
   	{
   		relink_user = val;
   	}
   	else
   	{
   		cout << "relink_user missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("relink_pass",daten)) != "ERROR")
   	{
   		relink_pass = val;
   	}
   	else
   	{
   		cout << "relink_pass missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("user_access_denied",daten)) != "ERROR")
   	{
   		user_access_denied = val;
   	}
   	else
   	{
   		cout << "user_access_denied\n";
   		
   		return 0;
   	}

 		if ((val=getkey("user_login_success",daten)) != "ERROR")
   	{
   		user_login_success = val;
   	}
   	else
   	{
   		cout << "user_login_success\n";
   		
   		return 0;
   	}

 		if ((val=getkey("add_to_passive_port",daten)) != "ERROR")
   	{
   		add_to_passive_port = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "add_to_passive_port missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("add_to_passive_port",daten)) != "ERROR")
   	{
   		add_to_passive_port = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "add_to_passive_port missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("port_range_start",daten)) != "ERROR")
   	{
   		port_range_start = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "port_range_start missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("port_range_end",daten)) != "ERROR")
   	{
   		port_range_end = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "port_range_end missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("use_port_range",daten)) != "ERROR")
   	{
   		use_port_range = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "use_port_range missing\n";
   		
   		return 0;
   	}
	

 		if ((val=getkey("buffersize",daten)) != "ERROR")
   	{
   		buffersize = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "buffersize missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("buffersize",daten)) != "ERROR")
   	{
   		buffersize = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "buffersize missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("pending",daten)) != "ERROR")
   	{
   		pending = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "pending missing\n";
   		
   		return 0;
   	}

 		if ((val=getkey("connect_timeout",daten)) != "ERROR")
   	{
   		connect_timeout = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "connect_timeout missing\n";
   		
   		return 0;
   	}

   	if ((val=getkey("ident_timeout",daten)) != "ERROR")
   	{
   		ident_timeout = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "ident_timeout missing\n";
   		
   		return 0;
   	}

   	if ((val=getkey("read_write_timeout",daten)) != "ERROR")
   	{
   		read_write_timeout = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "read_write_timeout missing\n";
   		
   		return 0;
   	}

 
   	if ((val=getkey("admin_list",daten)) != "ERROR")
   	{
   		admin_list = val;
   	}
   	else
   	{
   		cout << "admin_list missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("use_fxptosite_list",daten)) != "ERROR")
    {
   		use_fxptosite_list = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "use_fxptosite_list missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("fxp_tosite_list",daten)) != "ERROR")
    {
   		fxp_tosite_list = val;
   	}
   	else
   	{
   		cout << "fxp_tosite_list missing\n";
   		
   		return 0;
   	}
		
		
		if ((val=getkey("uid",daten)) != "ERROR")
   	{
   		uid = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "uid missing\n";
   		
   		return 0;
   	}
   	
   	
   	if ((val=getkey("debug_logfile",daten)) != "ERROR")
   	{
   		debug_logfile = val;
   	}
   	else
   	{
   		cout << "debug_logfile missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("log_to_screen",daten)) != "ERROR")
   	{
   		log_to_screen = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "log_to_screen missing\n";
   		
   		return 0;
   	}
		
		if ((val=getkey("thread_limit",daten)) != "ERROR")
   	{
   		thread_limit = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "thread_limit missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("ssl_ascii_cache",daten)) != "ERROR")
   	{
   		ssl_ascii_cache = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "ssl_ascii_cache missing\n";
   		
   		return 0;
   	}
   	
   	if ((val=getkey("use_ssl_exclude",daten)) != "ERROR")
   	{
   		use_ssl_exclude = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "use_ssl_exclude missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("sslexclude_list",daten)) != "ERROR")
   	{
   		sslexclude_list = val;
   	}
   	else
   	{
   		cout << "sslexclude_list missing\n";
   		
   		return 0;
   	}
 		

 		if ((val=getkey("infocmd",daten)) != "ERROR")
   	{
   		infocmd = val;
   	}
   	else
   	{
   		cout << "infocmd missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("helpcmd",daten)) != "ERROR")
   	{
   		helpcmd = val;
   	}
   	else
   	{
   		cout << "helpcmd missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("admincmd",daten)) != "ERROR")
   	{
   		admincmd = val;
   	}
   	else
   	{
   		cout << "admincmd missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("tositecmd",daten)) != "ERROR")
   	{
   		tositecmd = val;
   	}
   	else
   	{
   		cout << "tositecmd missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("fromsitecmd",daten)) != "ERROR")
   	{
   		fromsitecmd = val;
   	}
   	else
   	{
   		cout << "fromsitecmd missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("sslexcludecmd",daten)) != "ERROR")
   	{
   		sslexcludecmd = val;
   	}
   	else
   	{
   		cout << "sslexcludecmd missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("site_closed",daten)) != "ERROR")
   	{
   		site_closed = val;
   	}
   	else
   	{
   		cout << "site_closed missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("site_full",daten)) != "ERROR")
   	{
   		site_full = val;
   	}
   	else
   	{
   		cout << "site_full missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("reloadcmd",daten)) != "ERROR")
   	{
   		reloadcmd = val;
   	}
   	else
   	{
   		cout << "reloadcmd missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("usecommands",daten)) != "ERROR")
   	{
   		usecommands = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "usecommands missing\n";
   		
   		return 0;
   	}
   	
   	if ((val=getkey("showconnectfailmsg",daten)) != "ERROR")
   	{
   		showconnectfailmsg = atoi(val.c_str());
   	}
   	else
   	{
   		cout << "showconnectfailmsg missing\n";
   		
   		return 0;
   	}
   	
   	if ((val=getkey("pidfile",daten)) != "ERROR")
   	{
   		pidfile = val;
   	}
   	else
   	{
   		cout << "pidfile missing\n";
   		
   		return 0;
   	}
 		
 		if ((val=getkey("connectfailmsg",daten)) != "ERROR")
   	{
   		connectfailmsg = val;
   	}
   	else
   	{
   		cout << "connectfailmsg missing\n";
   		
   		return 0;
   	}
 		
 		return 1;
	}

}