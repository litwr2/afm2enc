//AFMtoENC v1.05
//(C) Copyright 2008,2009 GNU GPL, written by Vladimir Lidovski
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <cctype>
#include "unidata.c"
#include "showglyphs.c"

#define max(x,y) ((x)>(y))?(x):(y)
using namespace std;

map<int, vector<string> > uni2names;
typedef vector<string>::const_iterator CI_VES;
typedef map<int, vector<string> >::const_iterator CI_U2N;
map<int, vector<pair<int, int> > > uni2uni;
typedef vector<pair<int, int> >::const_iterator CI_VEI;
typedef map<int, vector<pair<int,int> > >::const_iterator CI_U2U;
map<string, int*> cmap;
map<int, char> ucmap;
string enc, afn, rfn, pfn, ext = "ttf", encprefix;
ifstream fi;
ofstream fop, fo;
string sout[258];
map<string, char> afm_map;
int printmap, dump;
vector<string> afm_all;
map<string, vector<int> > name2unis;
map<int, string> uni2afm;

void getout() {
  CI_VES ci;
  string s;
  for (int i = 0; i < 256; i++) 
    if (cmap[enc][i] == 0 || uni2afm.find(cmap[enc][i]) == uni2afm.end())
      sout[i + 1] = "/.notdef";
    else 
      sout[i + 1] = "/" + uni2afm[cmap[enc][i]];
}

void init() {
  string s;
  for (int i = 0; i < sizeof(unidata)/sizeof(UD); i++) {
      uni2names[unidata[i].u].push_back(unidata[i].n);
      name2unis[unidata[i].n].push_back(unidata[i].u);
  }
  for (int i = 0; i < sizeof(unisubst)/sizeof(US); i++) {
     uni2uni[unisubst[i].u].push_back(pair<int,int>(unisubst[i].m, unisubst[i].a));
     uni2uni[unisubst[i].a].push_back(pair<int,int>(unisubst[i].m, unisubst[i].u));
  }
  sout[257] = "] def";
  while (!fi.eof()) {
    getline(fi, s);
    if (s.find("C ") != 0) 
      continue;
    int i = s.find("N ");
    i = s.find_first_not_of(" ", i + 2);
    string t = s.substr(i, s.find_first_of(" ;", i) - i);
    afm_map[t] = 1;
    if (name2unis.find(t) != name2unis.end())
      uni2afm[name2unis[t][0]] = t;
    else if (t.find("uni") == 0 && t.length() == 7) {
      istringstream is(t.substr(3));
      int i;
      is >> hex >> i;
      uni2afm[i] = t;
    }
    else if (t.find("U") == 0 && t.length() == 5) {
      istringstream is(t.substr(1));
      int i;
      for (i = 1; i < 5; ++i)
        if (!isxdigit(t[i]))
          continue;
      is >> hex >> i;
      uni2afm[i] = t;
    }
    else if (t.find("u") == 0 && t.length() == 6) {
      istringstream is(t.substr(1));
      int i;
      for (i = 1; i < 6; ++i)
        if (!isxdigit(t[i]))
          continue;
      is >> hex >> i;
      uni2afm[i] = t;
    }  
    afm_all.push_back(t);
  }
}

void initcmap() {  
  for (int i = 0; i < 256; i++) 
    if (cmap[enc][i] != 0 && uni2afm.find(cmap[enc][i]) == uni2afm.end()) {
      CI_U2U vp;
      if ((vp = uni2uni.find(cmap[enc][i])) != uni2uni.end()) {
        int m = 5, u = 0;
        for (int k = 0; k < vp->second.size(); ++k)
          if (vp->second[k].first < m && uni2afm.find(vp->second[k].second) != uni2afm.end()) 
             m = vp->second[k].first, u = vp->second[k].second;
        if (u != 0) {
          ucmap.erase(cmap[enc][i]);
          cmap[enc][i] = u;
          ucmap[u] = 1;
        }  
      }
    }
}

void show_u2u_one(CI_U2U i) {
  for (CI_VEI i1 = i->second.begin(); i1 != i->second.end(); ++i1) {
      cout << setw(4) << setfill('0') << uppercase << hex 
        << i->first << " " << i1->first << " " << i1->first << endl;
  }
}

void show_u2n_one(CI_U2N i) {
  for (CI_VES i1 = i->second.begin(); i1 != i->second.end(); ++i1) {
      cout << setw(4) << setfill('0') << uppercase << hex 
        << i->first << " " << *i1 << endl;
  }
}

void show_u2n() {
  for (CI_U2N i = uni2names.begin(); i != uni2names.end(); ++i) 
    show_u2n_one(i);    
}

string toupper(string s) {
  for (int i = 0; i < s.length(); i++)
    s[i] = toupper(s[i]);
  return s;
}

string setencname() {
  string n = afn;
  while (n.length() > 3) {
    int i = 0;
    if (n.length()%2) 
      n += 'o';
    for (; i < n.length(); i+=2)
      n[(i+1)/2] = (n[i] + n[i+1])%26 + 'a';
    n = n.substr(0, max((n.length() + 1)/2, 3));
  }
  return n;
}

void savemap1() {
  for (int i = 0; i < 11; i++)
    fop << fontmap[i] << endl;
}

void savemap2() {
  int n = 16, charcount = 0;
  fop << fontmap[11] << pfn << " <" << rfn << " <" 
       << afn << "." << ext << "}\n";
  fop << fontmap[12] << pfn << " at 14pt\n";
  fop << fontmap[13] << afn << " " << enc << "}\n";
  for (int i = 14; i < 21; i++)
    fop << fontmap[i] << endl;
  for (int k = 0; tt[k].t != ot4; k++)    
    if (enc == tt[k].n)
      n = 8;  /* 128 char tables */
  for (int k = 0; k < n; k++) {
    fop << "\\mthree{" << hex << k << "0}&";
    for (int k1 = 0; k1 < 16; k1++) {
       if (k1%2 == 0)  
         fop << "\\mone{"; 
       else
         fop << "\\mtwo{";
       fop << "\\char\"" << setw(2) << setfill('0') << uppercase << hex 
               << k*16 + k1 << "}{";
       if (sout[k*16 + k1 + 1] != "/.notdef") { 
         string s = sout[k*16 + k1 + 1].substr(1);
         for (int i = 0; i < s.length(); i++)
           if (s[i] == '_') {
             s.replace(i,1,"\\_");
             i++;
           }
         fop << s;
       }     
       fop << "}{";
       if (cmap.find(enc) != cmap.end()) {
         if (cmap[enc][k*16 + k1] != 0) 
           fop << setw(4) << cmap[enc][k*16 + k1];
       }
       else
         fop << dec << charcount++;       
       fop << "}";
       if (k1 != 15) 
         fop << "&";
       else
         fop << "\\cr\n" << fontmap[20] << endl;
    }
  }
  fop << "}}}\n";
}

void savemap3() {
  fop << "\\bye\n";  
}

void savemap() {
  savemap1();
  savemap2();
  savemap3(); 
}

void usage() {
    int i;
    cout << "Usage:\n";
    cout << "  afmtoenc AFMFILENAME -n ENCODINGNAME  [-p [EXT]] [-o OUTPUTFILE]\n";
    cout << "  afmtoenc AFMFILENAME -d [ENCNAMEPREFIX]  [-p [EXT]]\n";
    cout << "  afmtoenc -V\n";
    cout << "  afmtoenc --help\n";
    cout << "e.g., afmtoenc -n t1 Cyberbas.afm\n";
    cout << "    afmtoenc -n t2a Cyberbas.afm -p -o Cyberbas-t2a.enc\n";
    cout << "    afmtoenc -p ttf Cyberbas.afm -n t5\n";
    cout << "    afmtoenc Cyberbas.afm -d -p\n";
    cout << "The option -p causes the creation of a tex-file with font map for pdfTeX\n"; 
    cout << "EXT is a font filename extension: ttf (default), pfa, pfb, ...\n";
    cout << "ENCNAMEPREFIX sets filename prefix - default is based on AFMFILENAME\n";
    cout << "Supported encodings: ";
    for (i = 0; i < sizeof(tt)/sizeof(TT) - 1; i++)
        cout << tt[i].n << ", ";
    cout << tt[i].n << endl;
}

void error(int i) {
   switch (i) {
     case 0: cout << "afm2enc v1.05 written by Vladimir Lidovski, (C) Copyright 2008,2009 GNU GPL\n"; break;
     case 1: cerr << rfn << " cannot be opened\n"; break;
     case 2: cerr << "Unknown encoding\n"; break;
     case 3: cerr << afn << " is not found\n"; break;
     case 4: cerr << pfn + ".tex" << " cannot be opened\n";
     case 5: cerr << "Incorrect parameters\n"; break;
     case 6: cerr << "Incorrect number of parameters\n"; break; 
     case 7: cerr << "The name of the output file is missed\n"; break;
     case 8: cerr << "Encoding name is missed\n"; break;
     case 10: cerr << "The name of AFM-file is missed\n"; break;
     case 11: cerr << "AFMFILENAME is duplicated\n"; break;
     case 12: cerr << "-n or -d option is missed\n"; break;
     case 14: cerr << pfn + "-dump.tex" << " cannot be opened\n";
   }
   exit(i);
}

void setlig(int i1, int i2, int i3) {
   if (ucmap[i1] == 1 && ucmap[i2] == 1 && ucmap[i3] == 1 && uni2afm.find(i1) != uni2afm.end() && uni2afm.find(i2) != uni2afm.end() && uni2afm.find(i2) != uni2afm.end())
      fo << "% LIGKERN " + uni2afm[i2] + " " + uni2afm[i3] + " =: " + uni2afm[i1] + " ;\n";
}

void outenc() {
  sout[0] = "/" + afn + enc + " [";
  if (!dump) {
    setlig(0xBF, '?', '`'); //questiondown
    setlig(0xA1, '!', '`'); //exclamdown
    setlig(0x2013, '-', '-'); //endash
    setlig(0x2014, 0x2013, '-'); //emdash
    setlig(0xAB, '<', '<'); //<<
    setlig(0xBB, '>', '>'); //>>
    setlig(0xFB00, 'f', 'f'); //ff
    setlig(0xFB01, 'f', 'i'); //fi
    setlig(0xFB02, 'f', 'l'); //fl
    setlig(0xFB03, 0xFB00, 'i'); //ffi
    setlig(0xFB03, 0xFB00, 'i'); //ffl
    //setlig(0x201C, '\'', '\''); //''
    setlig(0x201C, 0x2018, 0x2018); //''
    //setlig(0x201D, '`', '`'); //``
    setlig(0x201D, 0x2019, 0x2019); //``
  }
  for (int i = 0; i < 258; i++)
    fo << sout[i] << endl;
}

int main (int argc, char *argv[]) {
  int i = 1;
  if (argc == 1 || string(argv[1]) == "--help") {
    usage(); 
    return 0;
  }
  if (string(argv[1]) == "-V") error(0);
  if (argc < 3) error(6);    
  while (argc > i) 
    if (argv[i][0] == '-') {
      if (argv[i][1] == 'n') 
        if (argc > i + 1) {
          enc = argv[i + 1];
          i += 2;
         }  
        else 
          error(8);
      else if (argv[i][1] == 'o')
        if (argc > i + 1) {
          rfn = argv[i + 1];
          i += 2;
         }  
        else 
          error(7);
      else if (argv[i][1] == 'd') {
         dump = ++i;
         if (argc > i && argv[i][0] != '-')
           encprefix = argv[i++];
       }       
      else if (argv[i][1] == 'p') {
         printmap = ++i;
         if (argc > i && argv[i][0] != '-')
           ext = argv[i++];
      } 
      else 
        error(5);
    }
    else if (afn.length() == 0)
      afn = argv[i++];
    else 
      error(11);
  if (afn.length() == 0) error(10);
  if (dump + enc.length() == 0) error(12);
  if (afn.substr(afn.length() - 4, 4) == ".afm")
    afn.erase(afn.length() - 4, 4);
  for (int i = 0; i < sizeof(tt)/sizeof(TT); i++)
    cmap[tt[i].n] = tt[i].t; 
  fi.open((afn + ".afm").c_str());
  if (!fi) error(3);
  init();    
  fi.close();
  if (enc.length() > 0) {
    if (rfn.length() == 0)
      rfn = afn + "-" + enc + ".enc";
    else 
      if (rfn.length() < 5 || rfn.substr(rfn.length() - 4, 4) != ".enc")
        rfn += ".enc";
    enc = toupper(enc);
    if (cmap.find(enc) == cmap.end()) error(2);
    for (int i = 0; i < 256; i++)
      ucmap[cmap[enc][i]] = 1;
    initcmap();
    getout();
    fo.open(rfn.c_str());
    if (!fo) error(1);
    outenc();
    fo.close();
    if (printmap) {
      pfn = rfn;
      pfn.erase(pfn.length() - 4, 4);
      fop.open((pfn + ".tex").c_str());
      if (!fop) error(4);
      savemap();
      fop.close();
    }
  }
  if (dump) {
    if (encprefix.length() == 0)
      encprefix = setencname();
    if (printmap) {
      pfn = afn + "-" + encprefix;
      fop.open((pfn + "-dump.tex").c_str());
      if (!fop) error(14);
      savemap1();
     }
    for (int k = 0; k < (afm_all.size()+255)/256; k++) {
      ostringstream os;
      string save_pfn = pfn;
      os << k;
      enc = encprefix + os.str();
      rfn = afn + "-" + enc + ".enc";
      pfn += os.str();
      for (int i = 1; i < 257; i++)
        if (k*256 + i < afm_all.size() + 1)
          sout[i] = "/" + afm_all[k*256 + i - 1];
        else
          sout[i] = "/.notdef";
      fo.open(rfn.c_str());
      if (!fo) error(1);
      outenc();
      fo.close();
      if (printmap) {
        savemap2();
        fop << "\\vfill\\eject\n";
      }
     pfn = save_pfn;
    }
    if (printmap) {
      savemap3();
      fop.close();
    }
  }
  //show_u2n_one(uni2names.find(0x00ab));
  //show_u2u_one(uni2uni.find(0x00ab));
}
