import sqlite_blaster_python

col_names = ["key", "value"]
sqib = sqlite_blaster_python.sqlite_index_blaster(2, 1, col_names, "imain", 4096, 40000, "pytest.db")

print(sqib.put_string("test1", "test2"))
print(sqib.put_string("test1", "test2"))
print(sqib.put_string("test2", "test1"))
sqib.put_string("test3", "test1 klajs fdalk sjf;lk j;las jfl;ks j ;fj as;lk jfds;lk jdfs;a j ;sdlajf;laj ;dj f;ajfja s;fja; sfj;sjf; jsf; jsf; ajs;lf jas fjs;f js; jf; jf;j sf;lk ajsf; js;f jas;fj a;l sfj;a sjf;a jsf;l jsf; jas fjs;lk jfalksjf ;lajs f;lasj;lj g;lj;lgj l;kgj lk;gjldkjg lkdjg lsd jg;lsj gl;k dsjgkl;jsdglk;jd;lkjdslk;hl;kj;ldgjd;lsg;ls djgl  s gfj glkfdjg ;lkjs l jgs;ljfdg;l jsd;lg j g;sdjgl; sdjglk sdlg dslkgj dsl;kj g;ldsj glkdjs glk;j ;lk34jt6;j345lk34jlk salk;jrtwo ejgpoi jp9z ja43j5jlk jdslkjljgw slgm  w;lke gjs ;lkjg ;lksj dfgl;kjsdlgkjsd;jsdgjsd l3;l5 3;lj5 3lj5 3;l5j ;3lj53 5lj 3 53kl;j53lj5;3l5;35 3j 5j34 ;53j 5;l j35; 34 5 34 53 j5;l 35 j3j 53;l j5;j;l k;l aj;jlk;t 2 n5t3tn3lk; nlerngl;knerwsl;gknwenge wngneelktj eljtw tjwet jw ")

print(sqib.get_string("test3"))
print(sqib.get_string("test2"))
print(sqib.get_string("test1"))
