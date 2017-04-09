using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NSCC.Compiler
{
    abstract class AST
    {
        public int lineno;
    }

    class BinaryNode : AST
    {
        private AST lhs;
        private string op;
        private AST rhs;

        public BinaryNode(AST lhs, string op, AST rhs)
        {
            this.lhs = lhs;
            this.op = op;
            this.rhs = rhs;
        }
    }

    class UnaryNode : AST
    {
        private string op;
        private AST rhs;

        public UnaryNode(string op, AST rhs)
        {
            this.op = op;
            this.rhs = rhs;
        }
    }

    class NumberNode : AST
    {
        private double val;

        public NumberNode(double val)
        {
            this.val = val;
        }
    }

    class IdentifierNode : AST
    {
        private string id;

        public IdentifierNode(string id)
        {
            this.id = id;
        }
    }

    class CharNode : AST
    {
        private char val;

        public CharNode(char val)
        {
            this.val = val;
        }
    }

    class StringNode : AST
    {
        private string val;

        public StringNode(string val)
        {
            this.val = val;
        }
    }
}
