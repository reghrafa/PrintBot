using System;
using System.IO;
using Newtonsoft.Json;
using System.Collections.Generic;

namespace PrintBot.Domain
{
    class BordEditor_ModulPhysical
    {
        public string Name { get; set; }

        public List<BordEditor_PysicalPin> PinList { get; set; }

        public BordEditor_ModulPhysical()
        {
            PinList = new List<BordEditor_PysicalPin>();
        }

        public void saveModul()
        {
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            string filename = Path.Combine(path, $"Modul_{this.Name}.txt");
            using (var streamWriter = new StreamWriter(filename, false))
            {
                var json = JsonConvert.SerializeObject(this);
                streamWriter.WriteLine(json);
            }
        }

        public static BordEditor_ModulPhysical LoadModul(string filename)
        {
            BordEditor_ModulPhysical tmp = new BordEditor_ModulPhysical();

            string path = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            filename = Path.Combine(path, $"{filename}.txt");
            using (var streamReader = new StreamReader(filename))
            {
                string content = streamReader.ReadToEnd();
                tmp = JsonConvert.DeserializeObject<BordEditor_ModulPhysical>(content);
            }
            return tmp;
        }

    }
}