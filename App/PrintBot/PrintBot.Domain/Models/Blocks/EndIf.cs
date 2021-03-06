﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class EndIf : EndBlock, IBlock
    {
        public string CodeText
        {
            get
            {
                return "}";
            }
        }
        public int EndBlockPosition { get; set; }
        public string Name { get; set; }
        public string Text { get; set; }
        public bool IsChild { get; set; }

        public EndIf()
        {
            Name = "End If";
        }
    }
}
