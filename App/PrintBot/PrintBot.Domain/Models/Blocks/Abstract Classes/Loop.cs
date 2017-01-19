using PrintBot.Domain.Models.Blocks;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models.Blocks
{
    public abstract class Loop : StartBlock, IBlock
    {
        public string Name { get; set; }
        public int EndBlockPosition { get; set; }
        public string CodeText { get; set; }
        public string CodeBody { get; set; }
        public string Text { get; set; }
        public bool IsChild { get; set; }

        public void SetCodeText(string s)
        {
            CodeText = s;
        }
    }
}