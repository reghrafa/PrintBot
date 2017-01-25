using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Util;
using Android.Views;
using Android.Widget;
using System.Collections.ObjectModel;
using PrintBot.Droid.Controls;
using PrintBot.Droid.Controls.Blocks;
using PrintBot.Domain.Models.Blocks;
using PrintBot.Infrastructure.Services;
using PrintBot.Infrastructure.ViewModels;
using PrintBot.Droid.Activities;
using static Android.Views.View;

namespace PrintBot.Droid
{
    public class FragmentWorkspace : Fragment
    {
        BlockListController _blockListController = ServiceLocator.Current.BlockListController;
        DraggableListView listView;
        DraggableListAdapter adapter;
        int oldPos;
        bool blockAdded = false;
        BlockListItem draggedView;
        ImageView deleteButton;
        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            adapter = new DraggableListAdapter(this.Activity, _blockListController.List);
        }

        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.fragment_workspace, container, false);
            deleteButton = view.FindViewById<ImageView>(Resource.Id.workspace_delete_button);
            deleteButton.Visibility = ViewStates.Gone;
            listView = view.FindViewById<DraggableListView>(Resource.Id.workspace_listView);
            listView.Adapter = adapter;
            listView.ReorderingEnabled = true;
            listView.Drag += HandleDrag;
            return view;
        }

        void HandleDrag(object sender, View.DragEventArgs e)
        {
            var evt = e.Event;

            switch (evt.Action)
            {
                case DragAction.Started:
                    /* To register your view as a potential drop zone for the current view being dragged
                     * you need to set the event as handled
                     */
                    e.Handled = true;
                    //deleteButton.Visibility = ViewStates.Visible;
                    /* An important thing to know is that drop zones need to be visible (i.e. their Visibility)
                     * property set to something other than Gone or Invisible) in order to be considered. A nice workaround
                     * if you need them hidden initially is to have their layout_height set to 1.
                     */

                    break;
                case DragAction.Entered:
                    //if (!blockAdded)
                    //{
                    //    oldPos = _blockListViewController.List.Count;
                    //    var block = (BlockListItem)e.Event.LocalState;
                    //    var tmpBlock = block.GetAnInstanceAndInitialize();
                    //    _blockListViewController.List.Add(tmpBlock);
                    //    draggedView = tmpBlock;
                    //    if (tmpBlock.BlockType == BlockListItem.BlockTypeEnum.IfBlock)
                    //    {
                    //        _blockListViewController.List.Add(block.GetAnInstanceOfElseBlock());
                    //    }
                    //    if (tmpBlock.BlockHolder.Block is StartBlock)
                    //    {
                    //        _blockListViewController.List.Add(block.GetAnInstanceOfEndBlock(block.BlockType));
                    //    }
                    //}
                    //blockAdded = true;
                    break;
                case DragAction.Exited:
                    /* These two states allows you to know when the dragged view is contained atop your drop zone.
                     * Traditionally you will use that tip to display a focus ring or any other similar mechanism
                     * to advertise your view as a drop zone to the user.
                     */

                    break;
                case DragAction.Drop:
                    /* This state is used when the user drops the view on your drop zone. If you want to accept the drop,
                     *  set the Handled value to true like before.
                     */
                    e.Handled = true;
                    /* It's also probably time to get a bit of the data associated with the drag to know what
                     * you want to do with the information.
                     */
                    var block = (BlockListItem)e.Event.LocalState;
                    // Get the Position in the List
                    var position = listView.PointToPosition((int)e.Event.GetX(), (int)e.Event.GetY());
                    _blockListController.InsertBlockToList(Activity, block, position);
                    blockAdded = false;
                    break;
                case DragAction.Ended:
                    /* This is the final state, where you still have possibility to cancel the drop happened.
                     * You will generally want to set Handled to true.
                     */
                    e.Handled = true;
                    deleteButton.Visibility = ViewStates.Gone;
                    break;
                case DragAction.Location:
                    //int pos = listView.PointToPosition((int)e.Event.GetX(), (int)e.Event.GetY());
                    //HandleCellSwitch(pos);
                    //e.Handled = true;
                    break;
            }
        }

        private void HandleCellSwitch(int newPos)
        {
            if (newPos == -1)
            {
                newPos = _blockListController.List.Count - 1;
            }
            if (newPos != oldPos)
            {
                ((IDraggableListAdapter)listView.Adapter).SwapItems(oldPos, newPos, false);
                oldPos = newPos;
            }
        }
    }
}