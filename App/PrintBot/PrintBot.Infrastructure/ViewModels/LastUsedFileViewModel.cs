using PrintBot.Infrastructure.Mvvm;
using PrintBot.Domain.Models;
using System;
using System.IO;
using Newtonsoft.Json;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using PCLStorage;
using PrintBot.Infrastructure.Services;
using System.Collections.ObjectModel;
using PrintBot.Domain.Models.Blocks;

namespace PrintBot.Infrastructure.ViewModels
{
    public class LastUsedFileViewModel : ViewModelBase
    {


        private string _projectsFilename = "projects.json";
        private ObservableCollection<FileModel> _fileList = new ObservableCollection<FileModel>();
        private StorageService _storageService;

        public ObservableCollection<FileModel> FileList
        {
            get
            {
                return _fileList;
            }
            set
            {
                SetProperty(ref _fileList, value);
            }
        }

        public LastUsedFileViewModel()
        {
            _storageService = new StorageService();
        }


        public async Task LoadData()
        {
            if (await _storageService.FileExistsAsync(_projectsFilename))
            {
                var content = await _storageService.ReadFileAsync(_projectsFilename);
                FileList = JsonConvert.DeserializeObject<ObservableCollection<FileModel>>(content);
            }
        }

        public async Task<string> LoadFileData(string filename)
        {
            if (await _storageService.FileExistsAsync(filename))
            {
                var content = await _storageService.ReadFileAsync(filename);
                return content;
            }
            return null;
        }

        public string ChangeCreationDate(int position)
        {
            FileList[position].CreationDate = DateTime.Now;
            return FileList[position].FileName;
        }
        public void Sort()
        {
            FileList.OrderByDescending(a => a.CreationDate);
        }

        public async Task AddFile(string title)
        {
            var rand = new Random();
            var filename = title + ".c";
            await _storageService.WriteFileAsync(filename, "test" + rand.Next(123781));
            FileList.Insert(0, new FileModel() { Title = title, CreationDate = DateTime.Now, FileName = filename });
            await WriteAndRefresh();
        }
        public async Task Save()
        {
            await WriteAndRefresh();
        }
        

        private async Task WriteAndRefresh()
        {
            var json = JsonConvert.SerializeObject(FileList);
            await _storageService.WriteFileAsync(_projectsFilename, json);
        }

        private bool _openfile;
        public bool OpenFile
        {
            get { return _openfile; }
            set
            {
                if (_openfile != value) _openfile = value;
                OnPropertyChanged(nameof(OpenFile));
            }
        }
    }
}
