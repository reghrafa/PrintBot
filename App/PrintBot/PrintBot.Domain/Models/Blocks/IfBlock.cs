using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace PrintBot.Domain.Models.Blocks
{
    public class IfBlock : StartBlock, IBlock
    {
        private Condition condition;
        public string CodeText { get; set; }

        public bool IsStartBlock
        {
            get
            {
                return true;
            }
        }

        public IfBlock()
        {
            Equals equ = new Equals();
            condition = new Condition(1,3,equ);
            CodeText = "If (" + condition + ") {";
        }
        public bool IsChild { get; set; }

        public string Name
        {get;set;
        }

        public string Text
        {
            get; set;
        }
    }
}