using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace NSCC.Compiler
{
    struct OperatorTable
    {
        public static readonly string[] UNARY_OP_1 = { ".", "->" }; // also included [], especially
        public static readonly string[] UNARY_OP_2 = { "!", "+", "-", "~", "++", "--", "*", "&" }; // only implemented prefixed ++ --
        public static readonly string[] BINARY_OP_3 = { "*", "/", "%" };
        public static readonly string[] BINARY_OP_4 = { "+", "-" };
        public static readonly string[] BINARY_OP_5 = { "<<", ">>" };
        public static readonly string[] BINARY_OP_6 = { "<", ">", "<=", ">=" };
        public static readonly string[] BINARY_OP_7 = { "==", "!=" };
        public static readonly string[] BINARY_OP_8 = { "&" };
        public static readonly string[] BINARY_OP_9 = { "^" };
        public static readonly string[] BINARY_OP_10 = { "|" };
        public static readonly string[] BINARY_OP_11 = { "&&" };
        public static readonly string[] BINARY_OP_12 = { "||" };
        public static readonly string[] BINARY_OP_13 = { "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "~=", "<<=", ">>=" };
        public static readonly string[] BINARY_OP_14 = { "," };

        public static readonly char[] UNARY_OP_SYMBOL;
        public static readonly char[] BINARY_OP_SYMBOL;

        static OperatorTable()
        {
            {
                var unaryOpSymbolSet = new HashSet<char>();
                var properties = typeof(OperatorTable).GetProperties(BindingFlags.GetProperty | BindingFlags.Public | BindingFlags.Static);
                foreach (var p in properties)
                {
                    if (p.Name.StartsWith("UNARY") && p.PropertyType == typeof(string[]))
                    {
                        var source = p.GetValue(null) as string[];
                        foreach (var i in source)
                        {
                            foreach (var c in i)
                            {
                                unaryOpSymbolSet.Add(c);
                            }
                        }
                    }
                }
                UNARY_OP_SYMBOL = unaryOpSymbolSet.ToArray();
            }
            {
                var binaryOpSymbolSet = new HashSet<char>();
                var properties = typeof(OperatorTable).GetProperties(BindingFlags.GetProperty | BindingFlags.Public | BindingFlags.Static);
                foreach (var p in properties)
                {
                    if (p.Name.StartsWith("BINARY") && p.PropertyType == typeof(string[]))
                    {
                        var source = p.GetValue(null) as string[];
                        foreach (var i in source)
                        {
                            foreach (var c in i)
                            {
                                binaryOpSymbolSet.Add(c);
                            }
                        }
                    }
                }
                BINARY_OP_SYMBOL = binaryOpSymbolSet.ToArray();
            }
        }
    }
}
