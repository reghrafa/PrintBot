using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models
{
    public class FileModel
    {
        public string Title { get; set; }
        public DateTime CreationDate { get; set; }
        public string Path { get; set; }
    }
}