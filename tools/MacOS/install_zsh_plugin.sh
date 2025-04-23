#!/bin/zsh

# sunquan update at 2023-07-16 14:21

# 1 Copy Fonts
rsync -avh --progress Fonts/  ~/Library/Fonts/

# zsh plugin
# 1) zsh-autosuggestions
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

# vim ~/.zshrc
#plugins=(
#    # other plugins...
#    zsh-autosuggestions
#)
