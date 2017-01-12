using System;
using System.Collections.Generic;

namespace PrintBot.Domain.Models
{
    public class BordEditor_ModulPhysical
    {
        public string Name { get; set; }

        public string ModulType { get; set; }

        public List<BordEditor_PysicalPin> PinList { get; set; }

        public BordEditor_ModulPhysical()
        {
            PinList = new List<BordEditor_PysicalPin>();
        }
        
    }
}