using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NSCC
{
    struct CodeFile
    {
        public List<CodeLine> lines;
    }

    struct CodeLine
    {
        public int lineno;
        public List<CodeToken> tokens;
        public List<CodeError> errors;
    }

    struct CodeToken
    {
        public string value;
        public int colno;
    }

    struct CodeError
    {
        public string message;
        public int colno;
    }
}
