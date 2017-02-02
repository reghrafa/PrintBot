using PrintBot.Domain.Models.Blocks.Abstract_Classes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks
{
    public class RotateBlock : CommandBlock, IBlock
    {
        public string CodeText
        {
            get
            {
                return "SetRotationDegrees(" + Degrees.ToString() + ");";
            }
        }
        public int EndBlockPosition { get; set; }

        public bool IsChild { get; set; }

        public string Name { get; set; }

        public string Text { get; set; }

        public int Degrees { get; set; }

        public RotateBlock()
        {
            Name = "Rotate Block";
            Text = "Rotate by X degrees";
            Degrees = 90;
        }
    }
}
