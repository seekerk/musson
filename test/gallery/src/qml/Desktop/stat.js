function formatSize(size) {
    var count = 0
    while (size > 1024) {
        size /= 1024;
        count++;
        if (count >= 4) break;
    }

    size = size.toFixed(2);

    var ret = size + " ";
    switch(count) {
    case 0: ret += qsTr("byte(s)"); break;
    case 1: ret += qsTr("kilobyte(s)"); break;
    case 2: ret += qsTr("megabyte(s)"); break;
    case 3: ret += qsTr("gigabyte(s)"); break;
    case 4: ret += qsTr("terabyte(s)"); break;
        default:
            ret += "Unknown value(s)"; break;
    }

    return ret;
}
