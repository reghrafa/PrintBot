using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models.Blocks
{
    public interface IBlock
    {
        string CodeText { get; set; }
        int EndBlockPosition { get; set; }
        string Name { get; set; }
        string Text { get; set; }
        bool IsChild { get; set; }
    }
}