using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PrintBot.Infrastructure.Mvvm;
using PrintBot.Infrastructure.Services;
using System.Collections.ObjectModel;
using PrintBot.Domain.Models.Blocks;
using Newtonsoft.Json;

namespace PrintBot.Infrastructure.ViewModels
{
    public class CodeEditorViewModel : ViewModelBase
    {
        

        private StorageService _storageService;

        public CodeEditorViewModel()
        {
            _storageService = new StorageService();
        }

        public async Task SaveFile(string filename, ObservableCollection<IBlock> list)
        {
            string listOfBlocks = JsonConvert.SerializeObject(list, Formatting.Indented, new JsonSerializerSettings { TypeNameHandling = TypeNameHandling.Auto, TypeNameAssemblyFormat = System.Runtime.Serialization.Formatters.FormatterAssemblyStyle.Simple });
            await _storageService.WriteFileAsync(filename, listOfBlocks);
        }

        public async Task<string> LoadData(string filename)
        {
            if (await _storageService.FileExistsAsync(filename))
            {
                var content = await _storageService.ReadFileAsync(filename);
                return content;
            }
            return null;
        }
    }
}
