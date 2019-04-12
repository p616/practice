using System;

namespace ConsoleApp1
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine(Parse("1234"));
        }

        public string Reverse(string text)
        {
            if (text == null)
            {
                return null;
            }
            char[] array = text.ToCharArray();
            Array.Reverse(array);
            return new String(array);
        }

        static public int Parse(string input)
        {
            // at fn start
            if (String.IsNullOrEmpty(input) || String.IsNullOrWhiteSpace(input))
            {
                throw new Exception("empty string");
            }

            bool isNeg = (input[1] == '-');

            // after determining if first char is '-'
            if (isNeg)
            {
                input = input.Substring(1);
                // add
                if (input.Length == 0)
                {
                    throw new Exception("empty negative");
                }
            }

            int ret = 0;
            int mult = 1;

            // this was his suggestion when I started typing: for (int i=input.Length; i>=0; i--)
            //foreach (char c in input.Reverse())
            // this shouldn't compile
            // need Reverse fn above

            // fix
            char[] charArray = input.ToCharArray();
            Array.Reverse(charArray);
            input = new String(charArray);

            foreach (char c in input)
            {
                // in foreach loop
                if (!Char.IsDigit(c))
                {
                    throw new Exception("not a number");
                }

                // my code
                int num = (c - '0');
                ret += (num * mult);
                mult *= 10; 
            }

            return (isNeg ? ret * -1 : ret);
        }
    }
}

