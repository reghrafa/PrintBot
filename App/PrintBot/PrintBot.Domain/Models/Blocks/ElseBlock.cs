﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class ElseBlock : IBlock
    {
        public string CodeText
        {
            get
            {
                return "} else {";
            }
        }
        public int EndBlockPosition { get; set; }
        public bool IsChild { get; set; }

        public string Name { get; set; }

        public string Text { get; set; }

        public ElseBlock()
        {
            Name = "Else Block";
            Text = "Else Block";
            IsChild = false;
        }
    }
}
