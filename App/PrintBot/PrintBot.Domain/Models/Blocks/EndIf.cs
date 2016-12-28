﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class EndIf : EndBlock, IBlock
    {
        public string CodeText { get; set; }
        public string Name { get; set; }
        public string Text { get; set; }
        public bool IsChild { get; set; }

        public bool IsStartBlock
        {
            get
            {
                return false;
            }
        }

        public EndIf()
        {
            this.CodeText = "}";
        }
    }
}
