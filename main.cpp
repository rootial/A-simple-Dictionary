#include "air.h"
#include "solve.h"
#include <bits/stdc++.h>

using namespace std;
const int maxn = 111;
char s[maxn];
extern Trie solver;

bool readData() {
    FILE *f1 = fopen("index.txt", "r");
    int n, m;
    if(fscanf(f1, "%d", &n) == EOF ||n <= 0) {
        fclose(f1);
        return false;
    }
    vector<int> pos;
    solver.init();
    for(int i = 0; i < n; i++) {
        fscanf(f1, "%s%d", s, &m);
        pos.clear();
        int p;
        for(int j(0); j < m; j++) {
            fscanf(f1, "%d", &p);
            pos.pb(p);
        }
        solver.insert(s, 1, pos);
    }
    fclose(f1);
    return true;
}

bool writeData() {
    FILE *f1 = fopen("index.txt", "w");
    if(sz == 1) {
        fclose(f1);
        return false;
    }
    fprintf(f1, "%d\n", cntWord);
    dfs(0, "", f1);
    fclose(f1);
    return true;
}


void saveDict() {
    writeData();
}
void generateDict() {
    preProcess();
}
void exportDict() {
    readData();
}
string strPattern;
vector<string> inq;

vector<int>  inqprocess() {
    string tmp = gtk_entry_get_text((GtkEntry*)entry);
    inq.clear();
    for(int i = 0; i < (int)tmp.size(); ) {
        string word = "";
        int j;
        for(j = i; j < (int)tmp.size() && (isLetter(tmp[j]) || tmp[j] == '\'' || tmp[j] == '-'); j++) {
            word += tmp[j];
        }
        if(word != "")
            inq.pb(word);
        i = j+1;
    }

    return query(inq);
}
const int maxm = 10000;
char st[maxm], pattern[maxm];
string clearSen(string str) {
    string ans = "";
    strPattern.clear();
    int ok = 0;
    for(int i = 0; i < str.size(); i++) {
        if(str[i] == ' ' || str[i] == '\n') {
            if(ok == 0) {
                ans += ' ';
                ok = 1;
            }
        } else {
            ans += str[i];
            ok = 0;
        }
    }

    for(int i = 0; i < ans.size(); ) {
        if(isLetter(ans[i]) || ans[i] == '\'' || ans[i] == '-') {
            int j = i;
            string tmp = "";
            while(j < ans.size() && (isLetter(ans[j]) || ans[j] == '\'' || ans[j] == '-'))
                tmp += ans[j], strPattern += ' ', j++;
            for(int k = 0; k < tmp.size(); k++)
                tmp[k] = toLower(tmp[k]);
            for(int k = 0; k < tinq.size(); k++) {
                if(tinq[k] == tmp) {
                    for(int kk = i; kk < j; kk++)
                        strPattern[kk] = '_';
                    break;
                }
            }
            i = j;
        } else {
            strPattern += ' ';
            i++;
        }
    }
    return ans;
}
void resWin() {

    GtkWidget *window, *table, *label, *button, *scrolled_window;

    window = gtk_dialog_new();
    gtk_container_set_border_width(GTK_CONTAINER(window), 0);
    gtk_window_set_title(GTK_WINDOW(window), _("查询结果"));
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 0);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->vbox), scrolled_window, TRUE, TRUE, 0);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(window), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);

    table = gtk_table_new(13, 10, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 0);
    gtk_table_set_col_spacings(GTK_TABLE(table), 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), table);
    separator();
    gtk_table_attach_defaults(GTK_TABLE(table), hseparator[0], 0, 10, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), hseparator[1], 0, 10, 12, 13);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);

    vector<int> ans = inqprocess();
    sprintf(s, "%s%d%s\n", _("共找到"), ans.size(), _("个结果"));
    label = gtk_label_new(s);
    gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 10, 1, 2);
    gtk_label_set_line_wrap(GTK_LABEL(label),TRUE);//设置标签是否自动换行
    gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
    for(int i = 0; i < (int)ans.size() && i != 10; i++) {
        string tmp = clearSen(Sen[ans[i]-1]);
        int ct = (int)(log(i+1)/log(10))+1;
        ct++;
        while(ct--)
            strPattern = ' '+strPattern;
//            cout<<strPattern.size()<<endl;
        strcpy(pattern, strPattern.data());
        sprintf(st, "%d.%s\n", i+1, tmp.data());
        label = gtk_label_new(st);
        gtk_table_attach_defaults(GTK_TABLE(table), label, 0, 10, i+2, i+3);
        gtk_label_set_line_wrap(GTK_LABEL(label),TRUE);//设置标签是否自动换行
        gtk_label_set_justify(GTK_LABEL(label),GTK_JUSTIFY_LEFT);
        gtk_label_set_pattern(GTK_LABEL(label), pattern);
    }

    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    button = gtk_button_new_with_label(_("确认"));
    GTK_WIDGET_SET_FLAGS(GTK_WIDGET(button), GTK_CAN_DEFAULT);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(window)->action_area), button, TRUE, TRUE, 0);

    g_signal_connect_swapped(GTK_OBJECT(button), "clicked", G_CALLBACK(gtk_widget_destroy), window);
//    signal(window, "destroy", destroy, NULL);
    gtk_widget_grab_default(button);
    gtk_widget_show(window);
    gtk_widget_show_all(window);
}
void gtkWin() {

    GtkWidget *window,*table,*button[5];
    char *str[] = {_("生成词典"), _("导入词典"), _("保存词典"), _("退出"), "Go"};
    GTK_TABLE_START(1000,600);
    gtk_window_set_title(GTK_WINDOW(window), _("HH词典"));
    table = gtk_table_new(12,10,TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table),05);//设置组合表构件之间的空白行距
    gtk_table_set_col_spacings(GTK_TABLE(table),05);//设置组合表构件之间的空白列距

    separator();
    for(int i=0; i<5; i++) {
        button[i]=gtk_button_new_with_label(str[i]);
        if(i < 3)
            gtk_table_attach_defaults(GTK_TABLE(table),button[i], 0, 1, i+2, i+3);
        else  if(i == 3) gtk_table_attach_defaults(GTK_TABLE(table),button[i], 9, 10, 11, 12);
        else gtk_table_attach_defaults(GTK_TABLE(table),button[i], 9, 10, 0, 1);
    }

    entry = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), entry, 1, 8, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(table), vseparator[0], 1, 2, 2, 10);
    gtk_table_attach_defaults(GTK_TABLE(table), hseparator[1], 0, 10, 1, 2);
    gtk_table_attach_defaults(GTK_TABLE(table),hseparator[0], 0, 10, 10, 11);
    gtk_table_attach_defaults(GTK_TABLE(table),label_rights,0,2,11,12);
    signal(button[0], "clicked", generateDict, NULL);

    signal(button[1], "clicked", exportDict, NULL);

    signal(button[2], "clicked", saveDict, NULL);

    signal(button[3], "clicked", gtk_main_quit, NULL);

    signal(button[4], "clicked", resWin, NULL);

    GTK_TABLE_END;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    preProcess();
    gtkWin();
    return 0;
}
