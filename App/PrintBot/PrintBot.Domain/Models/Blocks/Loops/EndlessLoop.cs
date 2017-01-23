using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models.Blocks
{
    public class EndlessLoop : Loop
    {
        public new string CodeText
        {
            get
            {
                return "while (true) { ";
            }
        }
        public EndlessLoop()
        {
            Name = "Endless Loop";
            Text = "Dauerschleife";
            CodeBody = "";
            IsChild = false;
        }
    }
}