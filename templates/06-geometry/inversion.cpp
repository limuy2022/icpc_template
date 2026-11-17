Point invert_point(Point p,Point o,ld k) {
    Point v = p-o;
    return o+v*(k/norm2(v));
}
