using PrintBot.Domain.Models.Blocks.Abstract_Classes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class MoveBlock : CommandBlock, IBlock
    {
        public string CodeText { get; set; }
        public int EndBlockPosition { get; set; }

        public bool IsChild { get; set; }

        public string Name { get; set; }

        public string Text { get; set; }

        public int Acceleration { get; set; }

        public MoveBlock()
        {
            CodeText = "SomeMoveMethod()";
            Name = "Move Block";
            Text = "Moving on";
            Acceleration = 5;
        }
    }
}
