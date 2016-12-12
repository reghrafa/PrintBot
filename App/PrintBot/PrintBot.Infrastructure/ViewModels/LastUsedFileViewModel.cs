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

namespace PrintBot.Infrastructure.ViewModels
{
    public class LastUsedFileViewModel : ViewModelBase
    {
        public List<FileModel> FileList = new List<FileModel>();
        
        
        private string _projectsFilename = "projects.json";
        private StorageService _storageService;
        private IFolder _folder;

        public LastUsedFileViewModel()
        {
            _storageService = new StorageService();
        }
        
        public async Task LoadData()
        {
            var content = await _storageService.ReadFileAsync(_projectsFilename);
            FileList = JsonConvert.DeserializeObject<List<FileModel>>(content);
            OnPropertyChanged(nameof(FileList));
        }
        public string ChangeCreationDate(int position)
        {
            FileList[position].CreationDate = DateTime.Now;
            OnPropertyChanged(nameof(FileList));
            return FileList[position].Title;
        }
        public void Sort()
        {
            FileList.Sort((a, b) => b.CreationDate.CompareTo(a.CreationDate));
            OnPropertyChanged(nameof(FileList));
        }

        public async Task AddFile(string title)
        {
            var rand = new Random();
            var filename = title + ".c";
            await _storageService.WriteFileAsync(filename, "test" + rand.Next(123781));
            FileList.Insert(0, new FileModel() { Title = title, CreationDate = DateTime.Now, FileName = filename });
            OnPropertyChanged(nameof(FileList));
            await WriteAndRefresh();
        }
        public async Task Save()
        {
            await WriteAndRefresh();
        }


        private async Task WriteAndRefresh()
        {
            IFile file = await _folder.CreateFileAsync(_projectsFilename,
                CreationCollisionOption.ReplaceExisting);
            var json = JsonConvert.SerializeObject(FileList);
            await file.WriteAllTextAsync(json);
            OnPropertyChanged(nameof(FileList));
        }

        private bool _openfile;
        public bool OpenFile
        {
            get { return _openfile; }
            set
            {
                if (_openfile != value) _openfile = value;
                OnPropertyChanged("OpenFile");
            }
        }
    }
}
