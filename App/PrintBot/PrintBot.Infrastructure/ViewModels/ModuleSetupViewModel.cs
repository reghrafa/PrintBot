using Newtonsoft.Json;
using PrintBot.Domain;
using PrintBot.Domain.Models;
using PrintBot.Infrastructure.Services;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Infrastructure.ViewModels
{
    public class ModuleSetupViewModel
    {
        StorageService _storageService;
        
        public ModuleSetupViewModel(StorageService storageService)
        {
            _storageService = storageService;
        } 

        public async Task SaveModule(BordEditor_ModulPhysical module)
        {
            await _storageService.WriteFileAsync($"Modul_{module.Name}.txt", JsonConvert.SerializeObject(module));
        }

        public async Task<BordEditor_ModulPhysical> LoadModule(string filename)
        {
            var jsonModule = await _storageService.ReadFileAsync(filename);
            return JsonConvert.DeserializeObject<BordEditor_ModulPhysical>(jsonModule);
        }
    }
}
