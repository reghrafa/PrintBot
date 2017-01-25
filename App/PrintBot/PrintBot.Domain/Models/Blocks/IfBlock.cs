using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace PrintBot.Domain.Models.Blocks
{
    public class IfBlock : StartBlock, IBlock
    {
        public Condition Condition;
        public string CodeText
        {
            get
            {
                return "if (" + Condition.ToString + ") {";
            }
        }
        public int EndBlockPosition { get; set; }
    
        public IfBlock()
        {
            Equals equ = new Equals();
            Condition = new Condition(1, 1, equ);            
            Name = "If Block";
        }
        public bool IsChild { get; set; }

        public string Name
        {
            get; set;
        }

        public string Text
        {
            get; set;
        }
    }
}