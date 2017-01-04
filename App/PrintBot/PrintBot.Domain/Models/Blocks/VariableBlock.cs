using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class VariableBlock : IBlock
    {
        public string CodeText { get; set; }

        public bool IsChild { get; set; }

        public string Name { get; set; }

        public string Text { get; set; }

        public VariableBlock()
        {
            CodeText = "var x = 0";
            Name = "Variable Block";
            Text = "Keine Ahnung";
            IsChild = false;
        }
    }
}
