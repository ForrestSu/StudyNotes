#!/bin/zsh

# sunquan update at 2023-07-16 14:21

# 1 Copy Fonts
rsync -avh --progress Fonts/  ~/Library/Fonts/

# zsh plugin
# 1) zsh-autosuggestions
git clone https://github.com/zsh-users/zsh-autosuggestions ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-autosuggestions

# 2) zsh-syntax-highlighting
git clone https://github.com/zsh-users/zsh-syntax-highlighting.git ${ZSH_CUSTOM:-~/.oh-my-zsh/custom}/plugins/zsh-syntax-highlighting


# vim ~/.zshrc
#plugins=(
#   git
#   zsh-autosuggestions
#   zsh-syntax-highlighting
#   z
#   extract
#   catimg
# )
