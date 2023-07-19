
###### sunquan 2020-09-24 add
# current unix seconds
alias now='date +%s'
alias rcp='rsync -avh --progress'
# git
alias st='git status'
alias gp='git pull'
alias gl="git log --oneline --decorate --graph --all --pretty='%Cred%h%Creset -%C(auto)%d%Creset %s %Cgreen(%ad) %C(bold blue)<%an>%Creset' --date=short"
alias glods="git log --graph --pretty='%Cred%h%Creset -%C(auto)%d%Creset %s %Cgreen(%ad) %C(bold blue)<%an>%Creset' --date=short"
alias gsh='git show --pretty=fuller'
alias cloc='cloc --exclude-dir=.idea'
alias cloc_by_user='git ls-files | while read f; do git blame --line-porcelain $f | grep "^author "; done | sort -f | uniq -ic | sort -rn'
alias cloc_go="cloc --match-f '\.go$' --not-match-f '_test\.go$' "
alias cloc_gotest="cloc --match-f _test.go"
# tools
export LANG="en_US.UTF-8"
alias python=python3
alias sed=gsed
alias gdu='gdu-go'
alias time='/usr/bin/time -h'
alias ss='lsof -nP -i4TCP | grep LISTEN'
alias ll='ls -al -h'
alias rm_go_unit_test='find * -type f -name "*_test.go"|xargs rm'
alias go_build_linux='GOOS=linux GOARCH=amd64 go build -ldflags "-s -w" -v .'
alias k=kubectl
# PATH
export PATH="/usr/local/opt/openjdk@11/bin:$PATH"
export HOMEBREW_NO_ENV_HINTS=1
export PATH=$GOPATH/bin:$PATH
export PATH=/Users/sq/tools/bin:$PATH
# GoUP
export PATH=$HOME/.go/current/bin:$PATH

# Go
export GOPATH=/Users/sq/go_workspace
export GO111MODULE=on
export GOPRIVATE=""


