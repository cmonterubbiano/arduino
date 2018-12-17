#define  NA  1

#define  MAST 0

#define  NARD  3  //Numero di arduini configurabili
#define  NPUL  20 //Numero di pulsanti/arduino configurabili
#define  NREL  20 //Numero di     rele/arduino configurabili

byte  npul[NARD] ={ 6,  7,  5};  //Numero pulsanti arduino
byte  ppin[] ={  22, 24, 26, 28, 30,  0,  22,  24,  26,  28,  30,  32,  34,
                 22,  2,  3,  4,  5,  6};//Numero del pin
byte  appu[] ={  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0};//piastre  abbinamento pulsanti
byte  papu[] ={  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  1,  0,  0,
                 0,  0,  0,  0,  0,  0};//pulsanti abbinamento pulsanti
byte  sepu[] ={  0,  0,  0,  0,  0,  0,  4,  4,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0};//sequenza massima pulsanti 1-2-4-8-16-32 etc.
byte  psta[] ={  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                 0,  0,  0,  0,  0,  0};//stato attuale pulsanti
unsigned  long  spul[] ={  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                           0,  0,  0,  0,  0,  0};//Millisecondo inzio pressione pulsante                 
                
byte  rele[] ={  8,  5,  6}; //Numero rele arduino
byte  rpin[] ={ 23, 25, 27, 29, 31, 33, 35, 37,  23,  25,  27,  29,  31,
                 1,  2,  3,  4,  5,  6};//Numero del pin
byte  roff[] ={  1,  1,  1,  1,  1,  1,  1,  1,   1,   1,   0,   0,   1,
                 0,  0,  1,  0,  0,  0};//Status off rele
byte  rsta[] ={  0,  0,  0,  0,  0,  0,  0,  0,   0,   0,   0,   0,   0,
                 0,  0,  0,  0,  0,  0};// status attuale 0 off 1 on
                     

                 /* ogni riga rappresenta i potenziali 20 pulsanti di ogni piastra
                    i numeri corrispondono alla sequenza di accensione primo impulso 1
                    secondo 2 terzo 4 quarto  8  etc. etc.                              */
byte  rabb[(NARD * NREL)][(NARD *NPUL)] =
                    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 1 piastra 1
                     0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 2
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 3
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 4
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 5
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 6
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 7
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 8
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 9
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 10
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 11
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 12
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 13
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 14
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 15
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 16
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 17
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 18
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 19
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 20
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 1 piastra 2
                     5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 2
                     6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 3
                     0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 4
                     0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 5
                     0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 6
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 7
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 8
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 9
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 10
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 11
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 12
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 13
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 14
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 15
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 16
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 17
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 18
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 19
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 20
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 1 piastra 3
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 2
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 3
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 4
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 5
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 6
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 7
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 8
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 9
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 10
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 11
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 12
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 13
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 14
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 15
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 16
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 17
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 18
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 19
                     1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  // rele 20
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                     
byte  pulini;
byte  relini; 

#define  TMIN  10   //tempo minimo pressione pulsante in millesecondi
#define  TREP  3000 //tempo reset parziale ambiente
#define  TREG  10000//tempo reset generale

char  trasmi[NARD][256];
int  cambiostato =0;

void setup()
{
  int  iy;
  int  iz;
  
  for (iy =0; iy <NARD; iy++)
    *trasmi[iy] ='\0';
    
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(20);
  for (pulini =iy =0; iy <NA; pulini +=npul[iy], iy++);
  Serial.print("IX ");
  Serial.print(npul[NA]);
  Serial.print(" IZ ");
  Serial.println(pulini);
  iz =pulini;

  for (iy =iz +npul[NA]; iz <iy; iz++)
  {
    if (!ppin[iz])
      continue;
    pinMode(ppin[iz], INPUT);
    Serial.print("INPUT ");
    Serial.println(ppin[iz]);
    digitalWrite(ppin[iz], HIGH);
    delay(20);
  }

  for (iy =iz =0; iy <NA; iz +=rele[iy], iy++);
  relini =iz;
  Serial.begin(9600);
  for (iy =iz +rele[NA]; iz <iy; iz++)
  {
    if (!rpin[iz])
      continue;
      
    pinMode(rpin[iz], OUTPUT);
    Serial.print("OUTPUT ");
    Serial.print(rpin[iz]);
    delay(20);
    digitalWrite(rpin[iz], (!roff[iz] ? LOW : HIGH));
    Serial.print(" OFF ");
    Serial.println((!roff[iz] ? "LOW" : "HIGH"));
    delay(20);
  }
  Serial.println("FINE");
}

void loop()
{
  int  pulsa;
  int  ix;
  
  for(pulsa =0; pulsa <npul[NA]; pulsa++)
  {
    int  pulsante =pulini +pulsa;
    
    if (!ppin[pulsante])
      continue;
    
    int  pulstat =digitalRead(ppin[pulsante]);
    
    if (!pulstat)
    {
      if (!spul[pulsante])
      {
        spul[pulsante] =millis();
        Serial.print("ON Pulsante ");
        Serial.println(ppin[pulsante]);
      }
    }
    else  if (spul[pulsante])
    {
      unsigned  long  diff =(millis() -spul[pulsante]);
      
      spul[pulsante] =0;
      
      if (diff >TMIN  && diff <TREP)
      {
        int  esegui =1;
        
        Serial.print("Tempo normale ");
        Serial.print("Pulsante ");
        Serial.print(pulsante);
        Serial.print(" papu ");
        Serial.println(papu[pulsante]);
        if (papu[pulsante])
        {
          if (appu[pulsante] ==NA)
          {
            Serial.print("Pulsante prima ");
            Serial.print(ppin[pulsante]);
            psta[pulsante] =0;
            pulsante =(pulini +papu[pulsante] -1);
            Serial.print(" Pulsante dopo ");
            Serial.println(ppin[pulsante]);
          }
          else
          {
            char  mess[10];
             
            esegui =0;
            sprintf(mess, "p,%02d", papu[pulsante]);
            strcpy(&trasmi[(appu[pulsante])][(strlen(trasmi[(appu[pulsante])]))], mess);
            Serial.print("Stack da trasferire ");
            Serial.println(mess);
          }
        } 
        if (esegui)
        {
          int  posrel =((pulsante -pulini) +(NA *NREL));  // posizione del pulsante relativa.
          int  sequenza;
          int  iy;
          
          if (!psta[pulsante])
            psta[pulsante] =1;
          else
          {
            psta[pulsante] =(psta[pulsante] <<1);
            Serial.print(" Dopo shift ");
            Serial.println(psta[pulsante]);
            if (psta[pulsante] >sepu[pulsante])
              psta[pulsante] =0;
            Serial.print(" Dopo sequ ");
            Serial.println(psta[pulsante]);
          }
          sequenza =psta[pulsante];

          for (iy =0; iy <(NARD *NREL); iy++)
          {
            if (!rabb[iy][posrel])
              continue;
              
            Serial.print("iy ");
            Serial.print(iy);
            Serial.print(" posrel ");
            Serial.print(" rabb ");
            Serial.print(rabb[iy][posrel]);
            Serial.print(" sequenza ");
            Serial.print(sequenza);

            if (rabb[iy][posrel] & sequenza)
            {
              if (iy >=(NA *NREL) && iy <(NA * NREL +NREL))
              {
                int  nrel =rpin[(iy -(NA *NREL) +relini)];
                digitalWrite(nrel, (roff[(iy -(NA *NREL) +relini)] ? LOW : HIGH));
                rsta[(iy -(NA *NREL) +relini)] =1;
                Serial.print(" Rele HIGH ");
                Serial.println(nrel);
                cambiostato++;
              }
              else
              {
                char  mess[10];
             
                sprintf(mess, "r,%02d,1", (iy % NREL));
                strcpy(&trasmi[((iy /NREL))][(strlen(trasmi[((iy /NREL))]))], mess);
                Serial.print("Stack da trasferire ");
                Serial.println(mess);
              }
            }
            else
            {
              if (iy >=(NA *NREL) && iy <(NA * NREL +NREL))
              {
                int  nrel =rpin[(iy -(NA *NREL) +relini)];
                digitalWrite(nrel, (!roff[(iy -(NA *NREL) +relini)] ? LOW : HIGH));
                rsta[(iy -(NA *NREL) +relini)] =0;
                Serial.print(" Rele LOW ");
                Serial.println(nrel);
                cambiostato++;
              }
              else
              {
                char  mess[10];
             
                sprintf(mess, "r,%02d,0", (iy % NREL));
                strcpy(&trasmi[((iy /NREL))][(strlen(trasmi[((iy /NREL))]))], mess);
                Serial.print("Stack da trasferire ");
                Serial.println(mess);
              }
            }
          }
        }
      }
      else  if (diff >=TREP && diff <TREG)
      {
        Serial.print("Reset parziale ");
      }
      else  if (diff >=TREG)
      {
        Serial.print("Reset generale ");
      }
      Serial.print("Off Pulsante ");
      Serial.print(ppin[pulsante]);
      Serial.print(" tempo ");
      Serial.println(diff);
    }
    delay(1);
  }
  for (ix =0; ix <NARD; ix++)
  {
    if (*trasmi[ix])
	esegui_trasf(ix);
  }
  if (cambiostato && NA != MAST)
  {
    int  iy;
    
    Serial.print("Stato rele' -> ");
    for (iy =0,ix =relini; iy <rele[NA]; iy++)
    {
      Serial.print(rsta[(relini+iy)]);
      if (!iy)
        Serial1.print("X-");
      Serial1.print(rsta[(relini+iy)]);
    }
    Serial.println("");
    cambiostato =0;
  }
}

void esegui_trasf(int  ix)
{
  if (ix !=NA)
  {
    int  iy;
    
    Serial.print("Buffer da trasferire ");
    Serial.print(ix);
    Serial.print(" - ");
    Serial.println(trasmi[ix]);
    if (NA ==MAST)
      iy =ix;
    else  iy =NA;
    if (iy ==1)
    {
      Serial1.print(ix);
      Serial1.print("-");
      Serial1.println(trasmi[ix]);
    }
    if (iy ==2)
    {
      Serial2.print(ix);
      Serial2.print("-");
      Serial2.println(trasmi[ix]);
    }
  }
  else
  {
  }
  *trasmi[ix] ='\0';
}

void serialEvent1()
{
  while (Serial1.available())
  {
    char inChar = (char)Serial1.read();
    
    if (inChar == '\n')
      break;
    else  if (strlen(trasmi[NA])<256)
    {
      sprintf(&trasmi[NA][(strlen(trasmi[NA]))], "%c", inChar);
    }
    delay(10);
  }
  Serial.print("ARRIVATO -> ");
  Serial.println(trasmi[NA]); 
}
