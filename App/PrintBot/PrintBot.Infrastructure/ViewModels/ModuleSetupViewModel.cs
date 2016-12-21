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

        public async Task SaveModuleAsync(BordEditor_ModulPhysical module)
        {
            var modules = await LoadModuleAsync();
            BordEditor_ModulPhysical match = null;
            try
            {
                match = modules.Single(x => x.Name == module.Name);
            }
            catch (Exception) {}

            if (match != null)
            {
                match.PinList = module.PinList;
            }else
            {
                modules.Add(module);
            }

            String jason = JsonConvert.SerializeObject(modules);
            await _storageService.WriteFileAsync("module.json", jason);
            
        }

        public async Task<List<BordEditor_ModulPhysical>> LoadModuleAsync()
        {
            var jsonModule = "";
            // If module.json dose not exist create and load it it else load it strait away
            try
            {
                jsonModule = await _storageService.ReadFileAsync("module.json");
            }
            catch (Exception)
            {
                var tmp = JsonConvert.SerializeObject(new List<BordEditor_ModulPhysical>());
                await _storageService.WriteFileAsync("module.json", tmp);
                jsonModule = await _storageService.ReadFileAsync("module.json");
            }

            return JsonConvert.DeserializeObject<List<BordEditor_ModulPhysical>>(jsonModule);
        }
    }
}
