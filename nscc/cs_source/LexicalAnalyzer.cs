using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NSCC.Compiler
{
    public class LexParsingException : Exception
    {
        public LexParsingException(string message) : base(message) { }
    }

    unsafe static class LexicalAnalyzer
    {
        public static bool IsMemberOf(this char c, char[] group)
        {
            foreach(var i in group)
            {
                if(i == c)
                {
                    return true;
                }
            }
            return false;
        }

        public static bool IsMemberOf(this string c, string[] group)
        {
            foreach (var i in group)
            {
                if (i == c)
                {
                    return true;
                }
            }
            return false;
        }

        public static CodeFile Tokenize(string source)
        {
            var lines = source.Split('\n');
            CodeFile codeFile;
            foreach (var line in lines)
            {
                fixed (char* reader = line)
                {
                    codeFile
                }
            }
        }

        private static AST UnsafeTokenize(char* input)
        {
            var reader = &input;

            return ParseExp(reader);
        }

        private static AST ParseExp(char** reader)
        {
            var lhs = ParsePrimary(reader);
            while(true)
            {
                var op = IsOperator(reader);
                if(op != "")
                {
                    var rhs = ParsePrimary(reader);
                    return new BinaryNode(lhs, op, rhs);
                }
                else
                {
                    break;
                }
            }
            return lhs;
        }

        private static string IsOperator(char** reader)
        {
            var opBuilder = new StringBuilder();
            while (true)
            {
                if ((**reader).IsMemberOf(OPERATOR_CHAR))
                {
                    opBuilder.Append(**reader);
                    ++(*reader);
                }
                else
                {
                    break;
                }
            }
            var op = opBuilder.ToString();
            if (op.IsMemberOf(OPERATOR) || op == "")
            {
                return op;
            }
            else
            {
                throw new LexParsingException("Invalid operator at " + new string(*reader));
            }
        }

        private static AST ParsePrimary(char** reader)
        {
            if(**reader == '(')
            {
                var exp = ParseExp(reader);
                if(**reader == ')')
                {
                    return exp;
                }
                else
                {
                    throw new LexParsingException("Expect a \")\" at " + new string(*reader));
                }
            }
            else
            {
                if(char.IsDigit(**reader))
                {
                    return ParseNumber(reader);
                }
                else if(**reader == '"')
                {
                    return ParseString(reader);
                }
                else if(char.IsLetter(**reader))
                {
                    return ParseIdentifier(reader);
                }
                else
                {
                    throw new LexParsingException("Invalid symbol at " + new string(*reader));
                }
            }
        }

        private static AST ParseNumber(char** reader)
        {
            var numberBuilder = new StringBuilder();
            var floating = false;
            while(true)
            {
                if(char.IsDigit(**reader))
                {
                    numberBuilder.Append(**reader);
                    ++(*reader);
                }
                else if(!floating && **reader == '.')
                {
                    floating = true;
                    numberBuilder.Append(**reader);
                    ++(*reader);
                }
                else
                {
                    break;
                }
            }
            return new NumberNode(double.Parse(numberBuilder.ToString()));
        }

        private static AST ParseString(char** reader)
        {
            var escaping = false;
            var strBuilder = new StringBuilder();
            ++(*reader);
            while (**reader != '\0')
            {
                if (!escaping)
                {
                    if (**reader == '"')
                    {
                        break;
                    }
                    else if (**reader == '\\')
                    {
                        strBuilder.Append(**reader);
                        ++(*reader);
                        escaping = true;
                    }
                }
                else
                {
                    strBuilder.Append(**reader);
                    ++(*reader);
                    escaping = false;
                }
            }
            if(!escaping && **reader == '"')
            {
                ++(*reader);
                return new StringNode(strBuilder.ToString());
            }
            else
            {
                throw new LexParsingException("The string " + strBuilder + " ends up incorrectly");
            }
        }

        public static AST ParseIdentifier(char** reader)
        {
            var strBuilder = new StringBuilder();
            if(char.IsLetter(**reader) || **reader == '_')
            {
                strBuilder.Append(**reader);
                ++(*reader);
            }
            else
            {
                throw new LexParsingException("Invalid symbol at " + new string(*reader));
            }
            while (true)
            {
                if(char.IsLetterOrDigit(**reader) || **reader == '_')
                {
                    strBuilder.Append(**reader);
                    ++(*reader);
                }
                else
                {
                    break;
                }
            }
            return new IdentifierNode(strBuilder.ToString());
        }
    }
}
