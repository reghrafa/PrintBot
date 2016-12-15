using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace App2
{
    class CodeGenerator
    {
        private static int _whiteSpaceOffsetCount;

        public static string CreateCode(ObservableCollection<IBlock> list)
        {
            string output = "";
            _whiteSpaceOffsetCount = 0;

            foreach (IBlock item in list)
            {
                var code = item.CodeText;

                // special Case While
                if (item.GetType() == typeof(EndlessLoop))
                {
                    output += CreateOffsetWhile(code) + "\n"; // add to shown string
                }
                else
                {       //other Cases
                    output += SetOffset(item) + code + "\n";
                }
            }

            return output;
        }

        private static string SetOffset(IBlock item)
        {
            string whiteSpaceOffset = "";
            var itemType = item.GetType();

            //decrease offset amount
            if (itemType == typeof(EndLoop))
            {
                _whiteSpaceOffsetCount--;
            }

            //crate offset
            whiteSpaceOffset = CreateOffset();

            //increase offset amount
            if (itemType == typeof(IfBlock))
            {
                _whiteSpaceOffsetCount++;
            }

            //return offset
            return whiteSpaceOffset;
        }

        private static string CreateOffset()
        {
            string output = "";
            // create offset "  " * _whiteSpaceOffsetCount
            for (int i = 0; i < _whiteSpaceOffsetCount; i++)
            {
                output += "     ";
            }
            return output;
        }

        private static string CreateOffsetWhile(string code)
        {
            var whileComps = code.Split('\n'); // sperate lines
                                               //rebuild Code and add offset
            var tmp = ""; //place holder
            for (int i = 0; i < whileComps.Length; i++)
            {
                var prefix = i == 0 ? "" : "\n"; // if "while..." do nothing ELSE new line
                tmp += prefix + CreateOffset() + whileComps[i];
                if (i == 0)
                {
                    _whiteSpaceOffsetCount++; // set new offset
                }
            }
            return tmp;
        }

    }
}
