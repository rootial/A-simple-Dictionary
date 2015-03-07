#include "air.h"
#include <bits/stdc++.h>
using namespace std;
extern struct stemmer * create_stemmer(void) {
    return (struct stemmer *) malloc(sizeof(struct stemmer));
    /* assume malloc succeeds */
}

extern void free_stemmer(struct stemmer * z) {
    free(z);
}


static int cons(struct stemmer * z, int i) {
    switch (z->b[i]) {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return false;
    case 'y':
        return (i == 0) ? true : !cons(z, i - 1);
    default:
        return true;
    }
}


static int m(struct stemmer * z) {
    int n = 0;
    int i = 0;
    int j = z->j;
    while(true) {
        if (i > j) return n;
        if (! cons(z, i)) break;
        i++;
    }
    i++;
    while(true) {
        while(true) {
            if (i > j) return n;
            if (cons(z, i)) break;
            i++;
        }
        i++;
        n++;
        while(true) {
            if (i > j) return n;
            if (! cons(z, i)) break;
            i++;
        }
        i++;
    }
}


static int vowelinstem(struct stemmer * z) {
    int j = z->j;
    int i;
    for (i = 0; i <= j; i++) if (! cons(z, i)) return true;
    return false;
}


static int doublec(struct stemmer * z, int j) {
    char * b = z->b;
    if (j < 1) return false;
    if (b[j] != b[j - 1]) return false;
    return cons(z, j);
}

static int cvc(struct stemmer * z, int i) {
    if (i < 2 || !cons(z, i) || cons(z, i - 1) || !cons(z, i - 2)) return false;
    {
        int ch = z->b[i];
        if (ch  == 'w' || ch == 'x' || ch == 'y') return false;
    }
    return true;
}


static int ends(struct stemmer * z, char * s) {
    int length = s[0];
    char * b = z->b;
    int k = z->k;
    if (s[length] != b[k]) return false; /* tiny speed-up */
    if (length > k + 1) return false;
    if (memcmp(b + k - length + 1, s + 1, length) != 0) return false;
    z->j = k-length;
    return true;
}


static void setto(struct stemmer * z, char * s) {
    int length = s[0];
    int j = z->j;
    memmove(z->b + j + 1, s + 1, length);
    z->k = j+length;
}

static void r(struct stemmer * z, char * s) {
    if (m(z) > 0) setto(z, s);
}

static void step1ab(struct stemmer * z) {
    char * b = z->b;
    if (b[z->k] == 's') {
        if (ends(z,  (char*)"\04" "sses")) z->k -= 2;
        else if (ends(z, (char*)"\03" "ies")) setto(z, (char*)"\01" "i");
        else if (b[z->k - 1] != 's') z->k--;
    }
    if (ends(z, (char*)"\03" "eed")) {
        if (m(z) > 0) z->k--;
    } else if ((ends(z, (char*)"\02" "ed") || ends(z, (char*)"\03" "ing")) && vowelinstem(z)) {
        z->k = z->j;
        if (ends(z, (char*)"\02" "at")) setto(z, (char*)"\03" "ate");
        else if (ends(z, (char*)"\02" "bl")) setto(z, (char*)"\03" "ble");
        else if (ends(z, (char*)"\02" "iz")) setto(z, (char*)"\03" "ize");
        else if (doublec(z, z->k)) {
            z->k--;
            {
                int ch = b[z->k];
                if (ch == 'l' || ch == 's' || ch == 'z') z->k++;
            }
        } else if (m(z) == 1 && cvc(z, z->k)) setto(z, (char*)"\01" "e");
    }
}

static void step1c(struct stemmer * z) {
    if (ends(z, (char*)"\01" "y") && vowelinstem(z)) z->b[z->k] = 'i';
}

static void step2(struct stemmer * z) {
    switch (z->b[z->k-1]) {
    case 'a':
        if (ends(z, (char*)"\07" "ational")) {
            r(z, (char*)"\03" "ate");
            break;
        }
        if (ends(z, (char*)"\06" "tional")) {
            r(z, (char*)"\04" "tion");
            break;
        }
        break;
    case 'c':
        if (ends(z, (char*)"\04" "enci")) {
            r(z, (char*)"\04" "ence");
            break;
        }
        if (ends(z, (char*)"\04" "anci")) {
            r(z, (char*)"\04" "ance");
            break;
        }
        break;
    case 'e':
        if (ends(z, (char*)"\04" "izer")) {
            r(z, (char*)"\03" "ize");
            break;
        }
        break;
    case 'l':
        if (ends(z, (char*)"\03" "bli")) {
            r(z, (char*)"\03" "ble");    /*-DEPARTURE-*/
            break;
        }

        if (ends(z, (char*)"\04" "alli")) {
            r(z, (char*)"\02" "al");
            break;
        }
        if (ends(z, (char*)"\05" "entli")) {
            r(z, (char*)"\03" "ent");
            break;
        }
        if (ends(z, (char*)"\03" "eli")) {
            r(z, (char*)"\01" "e");
            break;
        }
        if (ends(z, (char*)"\05" "ousli")) {
            r(z, (char*)"\03" "ous");
            break;
        }
        break;
    case 'o':
        if (ends(z, (char*)"\07" "ization")) {
            r(z, (char*)"\03" "ize");
            break;
        }
        if (ends(z, (char*)"\05" "ation")) {
            r(z, (char*)"\03" "ate");
            break;
        }
        if (ends(z, (char*)"\04" "ator")) {
            r(z, (char*)"\03" "ate");
            break;
        }
        break;
    case 's':
        if (ends(z, (char*)"\05" "alism")) {
            r(z, (char*)"\02" "al");
            break;
        }
        if (ends(z, (char*)"\07" "iveness")) {
            r(z, (char*)"\03" "ive");
            break;
        }
        if (ends(z, (char*)"\07" "fulness")) {
            r(z, (char*)"\03" "ful");
            break;
        }
        if (ends(z, (char*)"\07" "ousness")) {
            r(z, (char*)"\03" "ous");
            break;
        }
        break;
    case 't':
        if (ends(z, (char*)"\05" "aliti")) {
            r(z, (char*)"\02" "al");
            break;
        }
        if (ends(z, (char*)"\05" "iviti")) {
            r(z, (char*)"\03" "ive");
            break;
        }
        if (ends(z, (char*)"\06" "biliti")) {
            r(z, (char*)"\03" "ble");
            break;
        }
        break;
    case 'g':
        if (ends(z, (char*)"\04" "logi")) {
            r(z, (char*)"\03" "log");    /*-DEPARTURE-*/
            break;
        }


    }
}


static void step3(struct stemmer * z) {
    switch (z->b[z->k]) {
    case 'e':
        if (ends(z, (char*)"\05" "icate")) {
            r(z, (char*)"\02" "ic");
            break;
        }
        if (ends(z, (char*)"\05" "ative")) {
            r(z, (char*)"\00" "");
            break;
        }
        if (ends(z, (char*)"\05" "alize")) {
            r(z, (char*)"\02" "al");
            break;
        }
        break;
    case 'i':
        if (ends(z, (char*)"\05" "iciti")) {
            r(z, (char*)"\02" "ic");
            break;
        }
        break;
    case 'l':
        if (ends(z, (char*)"\04" "ical")) {
            r(z, (char*)"\02" "ic");
            break;
        }
        if (ends(z, (char*)"\03" "ful")) {
            r(z, (char*)"\00" "");
            break;
        }
        break;
    case 's':
        if (ends(z, (char*)"\04" "ness")) {
            r(z, (char*)"\00" "");
            break;
        }
        break;
    }
}


static void step4(struct stemmer * z) {
    switch (z->b[z->k-1]) {
    case 'a':
        if (ends(z, (char*)"\02" "al")) break;
        return;
    case 'c':
        if (ends(z, (char*)"\04" "ance")) break;
        if (ends(z, (char*)"\04" "ence")) break;
        return;
    case 'e':
        if (ends(z, (char*)"\02" "er")) break;
        return;
    case 'i':
        if (ends(z, (char*)"\02" "ic")) break;
        return;
    case 'l':
        if (ends(z, (char*)"\04" "able")) break;
        if (ends(z, (char*)"\04" "ible")) break;
        return;
    case 'n':
        if (ends(z, (char*)"\03" "ant")) break;
        if (ends(z, (char*)"\05" "ement")) break;
        if (ends(z, (char*)"\04" "ment")) break;
        if (ends(z, (char*)"\03" "ent")) break;
        return;
    case 'o':
        if (ends(z, (char*)"\03" "ion") && z->j >= 0 && (z->b[z->j] == 's' || z->b[z->j] == 't')) break;
        if (ends(z, (char*)"\02" "ou")) break;
        return;
        /* takes care of -ous */
    case 's':
        if (ends(z, (char*)"\03" "ism")) break;
        return;
    case 't':
        if (ends(z, (char*)"\03" "ate")) break;
        if (ends(z, (char*)"\03" "iti")) break;
        return;
    case 'u':
        if (ends(z, (char*)"\03" "ous")) break;
        return;
    case 'v':
        if (ends(z, (char*)"\03" "ive")) break;
        return;
    case 'z':
        if (ends(z, (char*)"\03" "ize")) break;
        return;
    default:
        return;
    }
    if (m(z) > 1) z->k = z->j;
}

/* step5(z) removes a final -e if m(z) > 1, and changes -ll to -l if
   m(z) > 1. */

static void step5(struct stemmer * z) {
    char * b = z->b;
    z->j = z->k;
    if (b[z->k] == 'e') {
        int a = m(z);
        if (a > 1 || (a == 1 && !cvc(z, z->k - 1))) z->k--;
    }
    if (b[z->k] == 'l' && doublec(z, z->k) && m(z) > 1) z->k--;
}


extern int stem(struct stemmer * z, char * b, int k) {
    if (k <= 1) return k; /*-DEPARTURE-*/
    z->b = b;
    z->k = k; /* copy the parameters into z */


    step1ab(z);
    if (z->k > 0) {
        step1c(z);
        step2(z);
        step3(z);
        step4(z);
        step5(z);
    }
    return z->k;
}

