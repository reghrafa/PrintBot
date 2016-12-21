using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PrintBot.Infrastructure.Mvvm;
using PrintBot.Infrastructure.Services;

namespace PrintBot.Infrastructure.ViewModels
{
    public class CodeEditorViewModel : ViewModelBase
    {
        private string _codeString;
        public string CodeString { get { return _codeString; } private set { SetProperty(ref CodeString, _codeString, } }

        private StorageService _storageService;

        public CodeEditorViewModel()
        {
            _storageService = new StorageService();
        }
        
        public async Task LoadData()
        {
            if (await _storageService.FileExistsAsync(_projectsFilename))
            {
                var content = await _storageService.ReadFileAsync(_projectsFilename);
                FileList = JsonConvert.DeserializeObject<List<FileModel>>(content);
                OnPropertyChanged(nameof(FileList));
            }

        }
    }
}
