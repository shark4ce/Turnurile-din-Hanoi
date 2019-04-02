# Turnurile-din-Hanoi
Acest program rezolva jocul "Turnurile din Hanoi" folosind un numar mini de mutari( 2^n - 1) folosind Structuri de Date.

---Pentru rulare----:
terminal: make
          ./hannoi test5.in a.out



Programul primeste ca input un fisier ce contine un set de operatii, care pot fi:

1. add[culoare_disc][dimensiunea_disc] - operatie care adauga un disc de o anumita marime pe un primul turn (reprezentata ca stiva A, in ordine descrescatoare a diametrului, de sus in jos)
2.play[culoare][numar_mutari] - operatia cere simularea urmatoarelor [numar_mutari] ale jocului Hannoi de culoarea [culoare]
3.show[culoare] - operația cere afișarea stării curente a sistemului Hanoi de culoare [culoare]
- afișarea se va face pe 3 linii astfel:
A_[culoare]: diametru_disc_1 diametru_disc_2 ... diametru_disc_n
B_[culoare]: diametru_disc_1 diametru_disc_2 ... diametru_disc_n
C_[culoare]: diametru_disc_1 diametru_disc_2 ... diametru_disc_n

-- disc_1 este cel de la baza stivei, iar discul n este cel din vârful stivei.
4.show_moves[culoare][numar_mutari] - operația cere afișarea următoarelor [număr_mutări] mutări ce trebuie executate în
sistemul Hanoi de culoare [culoare] pentru a termina jocul. Dacă numărul total de mutări existente în sistem este mai mic decât [număr_mutări] se vor afișa doar cele existente.



---Detalii despre implementare-----

1.Initial am declarat niste structuri:
-structura *TLista pentru lista generica simplu inlantuita
-structura *TStiva,unde am varful stivei,care de fapt e inceputul unei liste de elemente(in cazul dat o lista de discuri), precum si un nume a acesteia, prin care voi diferentia stiva celor 3 turnuri dintr-un sistem hanoi.
-structura *Tcoada, in care de asemenea am varful inceputul unei cozi, care e de fapt inceputul unei liste de elemente(in cazul dat o lista in care voi salva miscarile necesare pentru a implementa jocul).
-structura *TCC, in care am 2 char-uri in care salvez numele turnurilor destinatie si sursa, ca sa stiu pe ce turn voi muta un disc si de pe ce turn voi scoate acest disc.
-structura *TSH, in care am un char*, in care salvez o anumita culoare cu ajutorul careia voi diferenta fiecare sistem hanoi in parte.Fiecare sistem hanoi are o singura culoare specifica ce il defineste.De asemenea, intr-un int salvez numarul de discuri din sistemul respectiv, precum si 3 elemente de tip TStiva, astfel definind in fiecare sistem hanoi 3 stive ce vor reprezenta cele 3 turnuri(A,B,C) si o variabila de tip TCoada in care voi salva miscarile neceare de efectuat pentru a implementa jocul.
2.Implementarea temei date consta in crearea unui liste simplu inlantuite generice de jocuri hanoi, a carei informatie o        castez la tipul de date TSH, astfel voi avea o lista de jocuri hanoi.In fiecare celula a acestei liste salvez culoarea ce     va fi caracteristica fiecarui sistem hanoi, 3 stive ce reprezinta cele 3 turnuri, care de fapt si ele reprezinta niste      liste generice simplu inlantuite a carei informatie o castez la tipul de date int ce va reprezenta raza fiecarui disc,        astfel fiecare celula a acestor liste vor reprezenta un disc.Mai creez si o coada care, de asemenea, reprezinta o lista       generica simplu inlantuita a carei informatie o castez la tipul de date TCC, in care voi salva miscarile necesare pentru     a implementa jocul(prin intermediul a 2 variabile de tip char).
3.Cu ajutorul functiei Push introducem o celula noua intr-o lista(introducem elemntele in cozile sau stivele noastre).
4.Functia Pop scoate diin Stiva primita ca parametru elemntul din varful acesteia.
5.Functiile check_heap si cheak_stack,verfica daca coada sau respectiv stiva este goala.
6.Functia alloc_cel_list alocam memorie pentru o celula din lista generica simplu inlantuita si introduce informatia in         aceasta.
7.Functia alloM_stack si allocM_heap aloca memoria necesara pentru stiva si respectiv coada noastra.
8.Functia allocM_system, aloca memorie pentru un sistem hanoi, precum si seteaza culoarea primita ca parametru specifica        fiecaruia.
9.Functia insert_ordonat insereaza ordonat crescator "discurile"(celula ce cotine informatia despre raza discului) primite ca    parametru pe "turnul"(stiva) primit ca parametru.Daca lista din stiva este goala, acesta il introduce la inceput.In caz      contrar avem mai multe optiuni:
   -daca elementul pe care vrem sa-l introducem este mai mic ca elementul din varful stivei ,elementul nou devine varful          stivei,iar acel element ce a fost varful stivei devine urmatorul acestuia.
    -in caz contrat, elementul nou trebuie inserat undeva prin mijlocul stivei la baza acestuia, astfel scoatem toate               elementele din stiva respectiva pana cand aceasta nu ramane goala si introducem nou element la baza sau pana cand unul       din elementele scoase nu e mai mare ca acesta,dupa care introducem ultimul element scos in stiva, dupa il introducem         elementul nou si dupa scoatem toate elementele din stiva auxiliara si le introducem inapoi in stiva respectiva.
10.Functia init_stack creaza lista cu elemente primite ca parametru pe care le introduce in stiva in ordine crescatoare         folosind functiile alloc_cel_list si insert_ordonat.
11.Functia print_stack initial scoate toate elementle din stiva primita ca parametru cu ajutorul functiei pop si le introduce    intr-o noua stiva auxiliara.Dupa care le scoate din stiva auxiliara si le introduce inapoi in stiva respectiva, doar ca de    fiecare data printeaza in fisier valoarea fiecarui element.
12.Functia init_stack_cel_system creeaza o noua celula care va fi de fapt un nou sistem hanoi, cu ajutorul functiilor           allocM_system si alloc_cel_list.
13.Functia find_color cauata in lista de siteme daca avem deja creat un sistem hanoi cu discuri de culoarea                      respectiva(culoarea este primita ca parametru) si returneaza adresa acestuia, daca nu returneaza null.
14.Functia show_culoare printeaza in fisierul primit ca parametru "discurile" din sistemul hanoi ce are drept descriptiv        aceasta culaore.
15.Functia add_new_systemH creeaza lista de sisteme si introduce in aceasta toate informatiile necesare.Daca lista de sisteme    e goala, atunci creeaza o noua celula in care introduce noul sistem hanoi dupa care introduce in celula de sisteme hanoi      "discurile" turnul A.Insa daca lista de sisteme nu e goala, acesta cu ajutorul functiei find_color, cauta daca un sistem       hanoi cu culoarea respectiva exista in lista de sisteme sau nu. Daca exista atunci acesta introduce "discul" in acesta,       daca nu, creeaza o noua celula in care introduce un nou sistem hanoi si adauga discul respectiv.
16.Functia calcul_algTower e o functie recursiva ce simuleaza algoritmul de implementare a jocului si introduce intr-o coada    toata miscarile necesare de efectuat.
17.Functia Extrq extrage elementele din coada, incepand cu inceputul acestuia.
18.Functia calculate_moves calculeaza miscarile necesare de efectuat pentru  a finisa jocul daca au fost facute deja cateva      mutari in sistemul hanoi.Daca pe turnul A si pe turnul B nu mai e nici un disc inseamna ca toate discurile se afla pe        discul C, ceea ce semnifica ca pentru acest sistem hanoi jocul a fost implementat pana la capat. Daca coada nu este nula      inseamna ca deja au fost efecuate careva mutari, respectiv au fost efectaute si schmbari in coada, astfel returnam coada      respectiva.Altfel, rezulta ca pentru acest sistem nu a fost aplicat nici o operatie de mutare si trebuie sa calculam          algoritmul de mutare de la inceput.
19.Functia print_moves printeaza miscarile necesare de efectuat intr-un sistem hanoi intr-un fisier primit ca                    parametru.Aceasta le afiseaza in timp ce le scoate si le introduce intr-o coada auxiliara.Dupa care le introduce inapoi in    coada initiala.
20.Functia show_movess calculeza mai intai miscarile necesare  cu ajutorul functiei calculate_moves si le printeza intr-un      fisier cu functia print_moves. 
21.Functia play efectueaza implicit miscarile de mutari in sistemul hanoi.Acesta initial calculeza calculeaza algoritmul pe     care trebuie sa-l efectueze cu ajutorul functiei calulcate_moves.Dupa care citeste din coada miscarile pe care trebuie sa     le efectueze si in fucntie de acestea scoate si introduce elemente in "turnurile" A,B si C.Dupa ce a efectuat un anumit       numaru de mutari acesta elimina din coada miscarile efectuate, astfel, in coada ramanand doar miscarile ce nu au fost         efectuate.
22.In main citim intr-un char* si unt int* datele din fisier si in functie de acestea apelam functiile necesare.
