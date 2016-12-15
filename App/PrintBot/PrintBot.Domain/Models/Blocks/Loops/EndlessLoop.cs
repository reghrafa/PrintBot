using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models.Blocks
{
    public class EndlessLoop : Loop
    {
        public EndlessLoop()
        {
            Name = "Endless Loop";
            Text = "Dauerschleife";
            CodeBody = "";
            IsChild = false;
            SetCodeText("while (true) { ");
        }
    }
}