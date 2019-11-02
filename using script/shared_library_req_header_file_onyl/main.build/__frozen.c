// This provides the frozen (compiled bytecode) files that are included if
// any.
#include <Python.h>

#include "nuitka/constants_blob.h"

// Blob from which modules are unstreamed.
#define stream_data constant_bin

// These modules should be loaded as bytecode. They may e.g. have to be loadable
// during "Py_Initialize" already, or for irrelevance, they are only included
// in this un-optimized form. These are not compiled by Nuitka, and therefore
// are not accelerated at all, merely bundled with the binary or module, so
// that CPython library can start out finding them.

struct frozen_desc {
    char const *name;
    ssize_t start;
    int size;
};

void copyFrozenModulesTo( struct _frozen *destination )
{
    struct frozen_desc frozen_modules[] = {
        { "base64", 6370599, 11253 },
        { "codecs", 6381852, 36628 },
        { "copy_reg", 6418480, 5137 },
        { "encodings", 6423617, -4362 },
        { "encodings.aliases", 6427979, 8760 },
        { "encodings.ascii", 6436739, 2253 },
        { "encodings.base64_codec", 6438992, 3829 },
        { "encodings.big5", 6442821, 1748 },
        { "encodings.big5hkscs", 6444569, 1788 },
        { "encodings.bz2_codec", 6446357, 4721 },
        { "encodings.charmap", 6451078, 3465 },
        { "encodings.cp037", 6454543, 2829 },
        { "encodings.cp1006", 6457372, 2915 },
        { "encodings.cp1026", 6460287, 2843 },
        { "encodings.cp1140", 6463130, 2829 },
        { "encodings.cp1250", 6465959, 2866 },
        { "encodings.cp1251", 6468825, 2863 },
        { "encodings.cp1252", 6471688, 2866 },
        { "encodings.cp1253", 6474554, 2879 },
        { "encodings.cp1254", 6477433, 2868 },
        { "encodings.cp1255", 6480301, 2887 },
        { "encodings.cp1256", 6483188, 2865 },
        { "encodings.cp1257", 6486053, 2873 },
        { "encodings.cp1258", 6488926, 2871 },
        { "encodings.cp424", 6491797, 2859 },
        { "encodings.cp437", 6494656, 8064 },
        { "encodings.cp500", 6502720, 2829 },
        { "encodings.cp720", 6505549, 2926 },
        { "encodings.cp737", 6508475, 8292 },
        { "encodings.cp775", 6516767, 8078 },
        { "encodings.cp850", 6524845, 7811 },
        { "encodings.cp852", 6532656, 8080 },
        { "encodings.cp855", 6540736, 8261 },
        { "encodings.cp856", 6548997, 2891 },
        { "encodings.cp857", 6551888, 7801 },
        { "encodings.cp858", 6559689, 7781 },
        { "encodings.cp860", 6567470, 8047 },
        { "encodings.cp861", 6575517, 8058 },
        { "encodings.cp862", 6583575, 8193 },
        { "encodings.cp863", 6591768, 8058 },
        { "encodings.cp864", 6599826, 8189 },
        { "encodings.cp865", 6608015, 8058 },
        { "encodings.cp866", 6616073, 8293 },
        { "encodings.cp869", 6624366, 8105 },
        { "encodings.cp874", 6632471, 2957 },
        { "encodings.cp875", 6635428, 2826 },
        { "encodings.cp932", 6638254, 1756 },
        { "encodings.cp949", 6640010, 1756 },
        { "encodings.cp950", 6641766, 1756 },
        { "encodings.euc_jis_2004", 6643522, 1812 },
        { "encodings.euc_jisx0213", 6645334, 1812 },
        { "encodings.euc_jp", 6647146, 1764 },
        { "encodings.euc_kr", 6648910, 1764 },
        { "encodings.gb18030", 6650674, 1772 },
        { "encodings.gb2312", 6652446, 1764 },
        { "encodings.gbk", 6654210, 1740 },
        { "encodings.hex_codec", 6655950, 3781 },
        { "encodings.hp_roman8", 6659731, 4112 },
        { "encodings.hz", 6663843, 1732 },
        { "encodings.idna", 6665575, 6368 },
        { "encodings.iso2022_jp", 6671943, 1801 },
        { "encodings.iso2022_jp_1", 6673744, 1817 },
        { "encodings.iso2022_jp_2", 6675561, 1817 },
        { "encodings.iso2022_jp_2004", 6677378, 1841 },
        { "encodings.iso2022_jp_3", 6679219, 1817 },
        { "encodings.iso2022_jp_ext", 6681036, 1833 },
        { "encodings.iso2022_kr", 6682869, 1801 },
        { "encodings.iso8859_1", 6684670, 2868 },
        { "encodings.iso8859_10", 6687538, 2883 },
        { "encodings.iso8859_11", 6690421, 2977 },
        { "encodings.iso8859_13", 6693398, 2886 },
        { "encodings.iso8859_14", 6696284, 2904 },
        { "encodings.iso8859_15", 6699188, 2883 },
        { "encodings.iso8859_16", 6702071, 2885 },
        { "encodings.iso8859_2", 6704956, 2868 },
        { "encodings.iso8859_3", 6707824, 2875 },
        { "encodings.iso8859_4", 6710699, 2868 },
        { "encodings.iso8859_5", 6713567, 2869 },
        { "encodings.iso8859_6", 6716436, 2913 },
        { "encodings.iso8859_7", 6719349, 2876 },
        { "encodings.iso8859_8", 6722225, 2907 },
        { "encodings.iso8859_9", 6725132, 2868 },
        { "encodings.johab", 6728000, 1756 },
        { "encodings.koi8_r", 6729756, 2890 },
        { "encodings.koi8_u", 6732646, 2876 },
        { "encodings.latin_1", 6735522, 2283 },
        { "encodings.mac_arabic", 6737805, 8014 },
        { "encodings.mac_centeuro", 6745819, 2937 },
        { "encodings.mac_croatian", 6748756, 2945 },
        { "encodings.mac_cyrillic", 6751701, 2935 },
        { "encodings.mac_farsi", 6754636, 2849 },
        { "encodings.mac_greek", 6757485, 2889 },
        { "encodings.mac_iceland", 6760374, 2928 },
        { "encodings.mac_latin2", 6763302, 4907 },
        { "encodings.mac_roman", 6768209, 2906 },
        { "encodings.mac_romanian", 6771115, 2946 },
        { "encodings.mac_turkish", 6774061, 2929 },
        { "encodings.palmos", 6776990, 3066 },
        { "encodings.ptcp154", 6780056, 4890 },
        { "encodings.punycode", 6784946, 7942 },
        { "encodings.quopri_codec", 6792888, 3647 },
        { "encodings.raw_unicode_escape", 6796535, 2202 },
        { "encodings.rot_13", 6798737, 3656 },
        { "encodings.shift_jis", 6802393, 1788 },
        { "encodings.shift_jis_2004", 6804181, 1828 },
        { "encodings.shift_jisx0213", 6806009, 1828 },
        { "encodings.string_escape", 6807837, 2061 },
        { "encodings.tis_620", 6809898, 2938 },
        { "encodings.undefined", 6812836, 2589 },
        { "encodings.unicode_escape", 6815425, 2150 },
        { "encodings.unicode_internal", 6817575, 2176 },
        { "encodings.utf_16", 6819751, 5160 },
        { "encodings.utf_16_be", 6824911, 1990 },
        { "encodings.utf_16_le", 6826901, 1990 },
        { "encodings.utf_32", 6828891, 5724 },
        { "encodings.utf_32_be", 6834615, 1883 },
        { "encodings.utf_32_le", 6836498, 1883 },
        { "encodings.utf_7", 6838381, 1883 },
        { "encodings.utf_8", 6840264, 1942 },
        { "encodings.utf_8_sig", 6842206, 4977 },
        { "encodings.uu_codec", 6847183, 4909 },
        { "encodings.zlib_codec", 6852092, 4641 },
        { "functools", 6856733, 6561 },
        { "locale", 6863294, 56524 },
        { "quopri", 6919818, 6544 },
        { "re", 6926362, 13363 },
        { "sre_compile", 6939725, 12522 },
        { "sre_constants", 6952247, 6177 },
        { "sre_parse", 6958424, 21076 },
        { "string", 6979500, 20349 },
        { "stringprep", 6999849, 14439 },
        { "struct", 7014288, 229 },
        { "types", 26012, 2703 },
        { NULL, 0, 0 }
    };

    struct frozen_desc *current = frozen_modules;

    for(;;)
    {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}
