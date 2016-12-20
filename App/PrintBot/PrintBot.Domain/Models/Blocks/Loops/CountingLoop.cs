using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace PrintBot.Domain.Models.Blocks
{
    public class CountingLoop : Loop
    {
        private int _amountOfLoops;
        public int AmountOfLoops
        {
            get
            {
                return _amountOfLoops;
            }
            set
            {
                _amountOfLoops = value;
            }
        }

        public CountingLoop()
        {
            Name = "Counting Loop";
            Text = "Zählschleife";
            AmountOfLoops = 5;
            CodeBody = "";
            SetCodeText("for (i = 0; i < " + AmountOfLoops.ToString() + "; i++) {");
        }
    }
}