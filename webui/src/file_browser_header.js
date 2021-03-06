import React from 'react'
import PropTypes from 'prop-types'
import Component from './component'
import FileBrowserModel, { rootPath } from './file_browser_model'
import PlaylistModel from './playlist_model'
import { Button } from './elements'
import urls from './urls'

export default class FileBrowserHeader extends Component
{
    constructor(props)
    {
        super(props);

        this.updateOn({ fileBrowserModel: 'change' });
        this.state = this.getStateFromModel();
        this.handleAddClick = this.handleAddClick.bind(this);
    }

    getStateFromModel()
    {
        const { currentPath, parentPath } = this.props.fileBrowserModel;
        return { currentPath, parentPath };
    }

    getParentLink()
    {
        var parentPath = this.state.parentPath;

        if (parentPath)
        {
            return { url:  urls.browsePath(parentPath), onClick: null };
        }
        else
        {
            return { url: null, onClick: e => e.preventDefault() };
        }
    }

    handleAddClick(e)
    {
        e.preventDefault();

        var currentPath = this.props.fileBrowserModel.currentPath;
        if (currentPath == rootPath)
            return;

        this.props.playlistModel.addItems([currentPath]);
    }

    render()
    {
        var parentLink = this.getParentLink();
        var title = this.state.currentPath == rootPath ? 'Music directories' : this.state.currentPath;

        return (
            <div className='panel-header'>
                <div className='header-block header-block-primary'>
                    <span className='header-label header-label-primary' title={title}>{title}</span>
                </div>
                <div className='header-block'>
                    <div className='button-bar'>
                        <Button
                            name='data-transfer-download'
                            onClick={this.handleAddClick}
                            title='Add current directory' />
                        <Button
                            name='home'
                            href={urls.browsePath(rootPath)}
                            title='Navigate to list of music directories' />
                        <Button
                            name='arrow-thick-top'
                            href={parentLink.url}
                            onClick={parentLink.onClick}
                            title='Navigate to parent directory' />
                    </div>
                </div>
            </div>
        );
    }
}

FileBrowserHeader.propTypes = {
    playlistModel: PropTypes.instanceOf(PlaylistModel).isRequired,
    fileBrowserModel: PropTypes.instanceOf(FileBrowserModel).isRequired
};

