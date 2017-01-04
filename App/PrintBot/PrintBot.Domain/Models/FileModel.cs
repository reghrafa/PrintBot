using PrintBot.Domain.Models.Blocks;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models
{
    public class FileModel
    {
        public string Title { get; set; }
        public DateTime CreationDate { get; set; }
        public string FileName { get; set; }
    }
}