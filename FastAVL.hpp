#ifndef FASTAVL_HPP
#define FASTAVL_HPP

#include <iostream>
#include <algorithm>

using namespace std;



/*
 *  ======================================================================
 *  Implementation eines nicht trivialen AVL-Baum-Konzeptes:
 *
 *  Die Knoten sollen nicht einfach nur einen Schlüssel enthalten dürfen,
 *  sondern auch zusätzliche Werte.
 *  Der Schlüssel ist in dem Knoten eingebettet,
 *  während die zusätzlichen Werte durch Vererbung erworben werden.
 *  ======================================================================
 */





/*
 *  Wer einen einfachen AVL-Baum nur mit Schlüsseln,
 *  aber ohne zusätzliche Werte benutzen möchte,
 *  kann einfach NoVal als Datentyp für die Werte verwenden.
 */
class NoVal
{
public:
    NoVal () {}
    void displayVal() {};
};





/*
 *  Die Intanzvariablen werden dem Baum zugänglich gemacht (friend),
 *  dazu muss die Baumklasse hier schon vordefiniert sein.
 */
template <typename Key, typename Val>
class AVL_Tree;





/*
 *  Der Knoten.
 *  Die Arbeitstiere sind hier als statische Methoden implementiert.
 *
 *  Von außen (öffentlich) lassen sich halt Knoten erzeugen und der Schlüssel abfragen.
 *  Um die zusätzlichen Werte (Val) wird sich nicht gekümmert.
 */
template <typename Key, typename Val>
class AVL_Node : public Val
{
    friend AVL_Tree<Key, Val>;

protected:
    AVL_Node*               smaller;
    AVL_Node*               greater;
    int                     balance;   // -1 .. +1 bei einem AVL-Baum
    Key                     key;

    static AVL_Node* find(AVL_Node* p, Key k);
    static bool rebalance (AVL_Node*& p, int offset, bool insertNotRemove);
    static bool insert (AVL_Node*& p, Key k, AVL_Node*& inserted);
    static bool remove (AVL_Node*& p, Key k);

    // Zu Testzwecken …
    static int calcHeight (AVL_Node* p);
    static void space8 (unsigned n = 1);
    static void display (AVL_Node* p, int h, char c = 'W');

public:
    AVL_Node (Key k);
    Key getKey ();
};





/*
 *  Der Baum.
 *  Quasi die GUI für obige Knoten;
 *  in diesem wird die Wurzel und die Höhe des Baums verwaltet.
 */
template <typename Key, typename Val>
class AVL_Tree
{
    using Node = AVL_Node<Key, Val>;

protected:
    Node*                   root;
    int                     height;

public:
    AVL_Tree ();
    int getHeight ();

    Node* find (Key k);
    Node* insert (Key k);
    void remove (Key k);
    Node* safeInsert (Key k);
    void safeRemove (Key k);

    // Zu Testzwecken …
    void check ();
    void display ();
};





/*
 *  ======================================================================
 *  Die statischen Knoten-Methoden
 *  ======================================================================
 */



/*
 *  Iterative Suche nach einem Schlüssel.
 */

template <typename Key, typename Val>
AVL_Node<Key, Val>* AVL_Node<Key, Val> :: find (AVL_Node* p, Key k)
{
    while (p != nullptr && k != p->key) {
        if (k < p->key) {
            p = p->smaller;
        }
        else {
            p = p->greater;
        }
    }
    return p;
}



/*
 *  Offset zur Balance des Knotens addieren und nötigenfalls
 *  (je nachdem, ob eingefügt oder gelöscht wird)
 *  den Knoten rebalancieren.
 *  Wird true zurückgegeben, setzt sich die Höhenänderung fort.
 */
template <typename Key, typename Val>
bool AVL_Node<Key, Val> :: rebalance (AVL_Node*& p, int offset, bool insertNotRemove)
{
    AVL_Node*               q;
    AVL_Node*               r;
    int                     bal;

    if (offset != -1 && offset != +1) {
        throw "Illegal offset while rebalancing!";
    }
    if (p == nullptr) {
        throw "Don’t rebalance empty trees!";
    }
    switch (p->balance += offset) {
    case  0:
        /*
         *  -1 => 0 or +1 => 0  —  AVL-Kriterium erfüllt
         *  Beim Einfügen hat der Unterbaum, in den eingefügt wurde,
         *  jetzt dieselbe Höhe wie der andere und die Höhenänderung wurde abgefangen.
         *  Beim Löschen wurde der vorher höhere Unterbaum auf dieselbe Höhe
         *  geschrumpft wie der andere und die Höhenänderung setzt sich fort.
         */
        return ! insertNotRemove;
    case -1:
    case +1:
        /*
         *  0 => -1 or 0 => +1  —  AVL-Kriterium erfüllt
         *  Beim Einfügen wächst einer der vorher gleich hohen Unterbäume
         *  und die Höhenänderung setzt sich fort.
         *  Beim Löschen behält einer der vorher gleich hohen Unterbäume
         *  seine Höhe und die Höhenänderung wurde abgefangen.
         */
        return insertNotRemove;
    case -2:
        /*
         *  Die Starthöhe bei dem Baum ist
         *  -   h     beim Einfügen   (hinterher h    =>  Höhenänderung abgefangen)
         *  -   h+1   beim Löschen    (hinterher h+1  =>  Höhenänderung abgefangen)
         *
         *  Neben den Knoten p, q, r ist die Höhe notiert
         *  sowie, in Klammern [(), <> or «»], die Balance.
         */
        q = p->smaller;
        if (q->balance <= 0) {   // == 0  =>  kann nur beim Löschen vorkommen
            /*
             *  Einfachrotation:
             *
             *      +-- p: h+1 (-2) --+          +- q: h   ( 0) -+
             *      |                 |          |     h+1 <+1>  |
             *      |                 |          |               |
             *   +- q: h (-1) -+     h-2        h-1           +- p: h-1 ( 0) -+
             *   |     h < 0>  |     h-2        h-1           |     h   <-1>  |
             *   |             |                              |               |
             *  h-1           h-2                            h-2             h-2
             *  h-1           h-1                            h-1             h-2
             */
            bal = q->balance;
            p->balance = (bal == 0) ? -1 : 0;   // ? <> : ()
            q->balance = (bal == 0) ? +1 : 0;   // ? <> : ()
            p->smaller = q->greater;
            q->greater = p;
            p = q;
            return (bal == 0) == insertNotRemove;
        }
        else {
            /*
             *  Doppelrotation:
             *
             *      +- p: h+1 (-2) ---+                +------ r: h ( 0) ------+
             *      |                 |                |                       |
             *      |                 |                |                       |
             *   +- q: h (+1) -+     h-2            +- q: h-1 ( 0) -+       +- p: h-1 <+1> -+
             *   |             |                    |     h-1 «-1»  |       |     h-1 ( 0)  |
             *   |             |                    |     h-1 ( 0)  |       |     h-1 ( 0)  |
             *  h-2         +- r: h-1 <-1> -+       |               |       |               |
             *              |     h-1 «+1»  |      h-2             h-2     h-3             h-2
             *              |     h-1 ( 0)  |                      h-3     h-2
             *              |               |                      h-2     h-2
             *             h-2             h-3
             *             h-3             h-2
             *             h-2             h-2
             */
            r = q->greater;
            bal = r->balance;
            p->balance = (bal == -1) ? +1 : 0;   // ? <> : ()
            q->balance = (bal == +1) ? -1 : 0;   // ? «» : ()
            r->balance = 0;
            p->smaller = r->greater;
            r->greater = p;
            q->greater = r->smaller;
            r->smaller = q;
            p = r;
            return ! insertNotRemove;
        }
    case +2:
        /*
         *  Das Ganze noch einmal spiegelverkehrt …
         */
        q = p->greater;
        if (q->balance >= 0) {   // == 0  =>  kann nur beim Löschen vorkommen
            bal = q->balance;
            p->balance = (bal == 0) ? +1 : 0;
            q->balance = (bal == 0) ? -1 : 0;
            p->greater = q->smaller;
            q->smaller = p;
            p = q;
            return (bal == 0) == insertNotRemove;
        }
        else {
            r = q->smaller;
            bal = r->balance;
            p->balance = (bal == +1) ? -1 : 0;
            q->balance = (bal == -1) ? +1 : 0;
            r->balance = 0;
            p->greater = r->smaller;
            r->smaller = p;
            q->smaller = r->greater;
            r->greater = q;
            p = r;
            return ! insertNotRemove;
        }
    default:
        throw "Desaster – irregular balance in tree!";
    }
}



/*
 *  Schlüssel k in Baum p einfügen.
 *  inserted enthält anschließend einen Zeiger auf den neuen Knoten.
 *  Es wird true zurückgegeben, solange sich die Höhenänderung fortsetzt.
 */
template <typename Key, typename Val>
bool AVL_Node<Key, Val> :: insert (AVL_Node*& p, Key k, AVL_Node*& inserted)
{
    if (p == nullptr) {
        inserted = p = new AVL_Node(k);   // neuen Knoten anlegen und zusätzlich in inserted merken
        if (inserted == nullptr) {
            throw "Out of memory!";
        }
        return true;   // Höhenänderung durch den neuen Knoten
    }
    else if (k < p->key) {
        if (insert(p->smaller, k, inserted)) {
            return rebalance(p, -1, true);
        }
    }
    else if (k > p->key) {
        if (insert(p->greater, k, inserted)) {
            return rebalance(p, +1, true);
        }
    }
    else {
        throw "Key to insert already in tree!";
    }
    return false;   // Höhenänderung abgefangen
}



/*
 *  Schlüssel k aus dem Baum p entfernen.
 */
template <typename Key, typename Val>
bool AVL_Node<Key, Val> :: remove (AVL_Node*& p, Key k)
{
    AVL_Node*                    q;
    AVL_Node*                    r;

    if (p == nullptr) {
        throw "Key to delete not in tree!";
    }
    else if (k < p->key) {
        if (remove(p->smaller, k)) {
            return rebalance(p, +1, false);
        }
    }
    else if (k > p->key) {
        if (remove(p->greater, k)) {
            return  rebalance(p, -1, false);
        }
    }
    else if (p->greater == nullptr || p->smaller == nullptr) {
        if (p->greater == nullptr) {   // Ist p->key bereits das größte Element im Baum?
            q = p->smaller;   // leerer Baum oder Blatt
        }
        else {
            q = p->greater;   // Blatt
        }
        delete p;   // Element löschen und
        p = q;   // durch Blatt (or leeren Baum) ersetzen
        return true;   // Höhenänderung durch das Löschen
    }
    else {
        q = p->greater;
        r = p;
        while (q->smaller != nullptr) {   // kleinstes Element im Unterbaum p->greater finden
            r = q;   // und auch den Vorgänger darauf merken
            q = q->smaller;
        }

        /*
         *  Da wir nichts über die ererbte Val-Struktur wissen,
         *  reicht es nicht, einfach die Schlüssel auszutauschen.
         *  Wir setzen stattdessen alle Zeiger und die Balance um.
         */
        if (r != p) {   // Der Vorgänger von q muss hinterher auf p zeigen,
            r->smaller = p;
        }
        else {   // selbst wenn es p selbst ist; nach dem Tausch von p und q wird q dann auf p zeigen.
            r->greater = p;
        }
        swap(p->smaller, q->smaller);
        swap(p->greater, q->greater);
        swap(p->balance, q->balance);
        swap(p, q);   // Bedeutung von p und q umsetzen

        if (remove(p->greater, k)) {   // und das gewünschte Element aus dem Unterbaum löschen
            return rebalance(p, -1, false);   // und Baum rebalancieren
        }
    }
    return false;   // Höhenänderung abgefangen
}



/*
 *  Höhe des Baum p tatsächlich berechnen;
 *  bricht mit Fehlermeldung ab,
 *  wenn AVL-Kriterium verletzt ist
 *  oder Balance und Höhe der Unterbäume nicht zueinander passen.
 */
template <typename Key, typename Val>
int AVL_Node<Key, Val> :: calcHeight (AVL_Node* p)
{
    if (p == nullptr) {
        return 0;
    }
    else {
        int h1 = calcHeight(p->smaller);
        int h2 = calcHeight(p->greater);
        if (abs(h1 - h2) > 1) {
            throw "No AVL tree!";
        }
        else if (h2 - h1 != p->balance) {
            throw "Balance not in line!";
        }
        return max(h1, h2) + 1;
    }
}



/*
 *  Gibt einfach n-mal acht Leerzeichen aus.
 *  Wer sich bei dem Namen der Methode an sense8 erinnert fühlt,
 *  könnte damit richtigliegen …
 */
template <typename Key, typename Val>
void AVL_Node<Key, Val> :: space8 (unsigned n)
{
    for (unsigned i = 0; i < n; i++) {
        cout << "        ";
    }
}



/*
 *  Zaubert eine Darstellung des Baums auf den Bildschirm.
 */
template <typename Key, typename Val>
void AVL_Node<Key, Val> :: display (AVL_Node* p, int h, char c)
{
    if (p != nullptr) {
        display(p->smaller, h-1, 'L');
        space8(h);
        cout << c << h << ": " << p->key << " (" << p->balance << ")";
        p->displayVal();
        cout << endl;
        display(p->greater, h-1, 'R');
    }
    else {
        space8(h);
        cout << c << h << ": " << "÷" << endl;
    }
}





/*
 *  ======================================================================
 *  Die nicht statischen Knoten-Methoden
 *  ======================================================================
 */



/*
 *  Konstruktor – Val muss einen Standard-Konstruktor anbieten
 */
template <typename Key, typename Val>
AVL_Node<Key, Val> :: AVL_Node (Key k) : Val ()
{
    smaller = nullptr;
    greater = nullptr;
    balance = 0;
    key     = k;
}



/*
 *  Wer extern den Schlüssel aus dem Knoten extrahieren will …
 */
template <typename Key, typename Val>
Key AVL_Node<Key, Val> :: getKey ()
{
    return key;
}





/*
 *  ======================================================================
 *  Die Baum-Methoden
 *
 *  rufen im Wesentlichen die entsprechenden Methoden
 *  aus der Knotenklasse mit der Wurzel als Parameter auf.
 *  ======================================================================
 */



/*
 *  Konstruktor
 */
template <typename Key, typename Val>
AVL_Tree<Key, Val> :: AVL_Tree ()
{
    root   = nullptr;
    height = 0;
}



/*
 *  Wer die Höhe wissen will …
 */
template <typename Key, typename Val>
int AVL_Tree<Key, Val> :: getHeight ()
{
    return height;
}



/*
 *  Schlüssel k in dem Baum suchen
 */
template <typename Key, typename Val>
AVL_Node<Key, Val>* AVL_Tree<Key, Val> :: find (Key k)
{
    return Node::find(root, k);
}



/*
 *  Schlüssel k in den Baum einfügen
 *  Schlüssel darf nicht schon im Baum enthalten sein
 *  Liefert einen Zeiger auf den neuen Knoten
 */
template <typename Key, typename Val>
AVL_Node<Key, Val>* AVL_Tree<Key, Val> :: insert (Key k)
{
    Node*                   inserted = nullptr;

    if (Node::insert(root, k, inserted)) {
        height++;
    }
    return inserted;
}



/*
 *  Schlüssel k aus dem Baum löschen
 *  Schlüssel muss ich im Baum befinden
 */
template <typename Key, typename Val>
void AVL_Tree<Key, Val> :: remove (Key k)
{
    if (Node::remove(root, k)) {
        height--;
    }
}



/*
 *  Es wird geprüft, ob sich der Schlüssel k bereits im Baum befindet,
 *  und nur, wenn nicht, insert aufgerufen.
 */
template <typename Key, typename Val>
AVL_Node<Key, Val>* AVL_Tree<Key, Val> :: safeInsert (Key k)
{
    Node*                   foundOrIserted;

    if ((foundOrIserted = find(k)) == nullptr) {
        foundOrIserted = insert(k);
    }
    return foundOrIserted;
}



/*
 *  Es wird geprüft, ob sich der Schlüssel k im Baum befindet,
 *  und nur dann remove aufgerufen.
 */
template <typename Key, typename Val>
void AVL_Tree<Key, Val> :: safeRemove (Key k)
{
    if (find(k) != nullptr) {
        remove(k);
    }
}



/*
 *  Macht sich zunutze, dass bei der Höhenberechnung die AVL-Struktur
 *  überprüft wird, und vergleicht das Ergebnis mit der gespeicherten Höhe.
 */
template <typename Key, typename Val>
void AVL_Tree<Key, Val> :: check ()
{
    if (Node::calcHeight(root) != height) {
        throw "Height not in line!";
    }
}



/*
 *  Ein bisschen Eye-Candy zu Testzwecken
 */
template <typename Key, typename Val>
void AVL_Tree<Key, Val> :: display ()
{
    Node::display(root, height);
    try {
        cout << endl << "AVL-Status: ";
        check();
        cout << "Good";
    } catch (const char* s) {
         cout << " – Aww!";
    }
    cout << endl << endl;
}





#endif // FASTAVL_HPP
