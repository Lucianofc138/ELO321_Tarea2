typedef struct board_zone {
    int init_row;
    int fin_row;
    int init_col;
    int fin_col;
} zone;

zone get_row_by_pos(int pos){
    zone row;
    row.init_col = 0;
    row.fin_col = 8;
    row.init_row = pos;
    row.fin_row = pos;
    return row;
}
zone get_col_by_pos(int pos){
    zone col;
    col.init_row = 0;
    col.fin_row = 8;
    col.init_col = pos;
    col.fin_col = pos;
    return col;
}
zone get_box_by_pos(int pos){
    zone box;
    box.init_col = (pos % 3)*3;
    box.init_row = ((int)(pos/3))*3;
    box.fin_col = box.init_col+2;
    box.fin_row = box.init_row+2;
    return box;
}

