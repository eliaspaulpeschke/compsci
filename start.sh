session='compsci'
exists=$(tmux list-sessions | grep $session)

if [ "$exists" = "" ]
then
    tmux new-session -d -s $session

    tmux rename-window -t 0 'emanote'
    tmux send-keys -t 'emanote' 'cd notes && emanote run --port 8080' C-m 
    tmux split-window -t $session:0 'firefox --new-tab http://localhost:8080'

    tmux new-window -t $session:1 -n 'notes-nvim'
    tmux send-keys -t 'notes-nvim' 'cd notes && nvim' C-m

    tmux new-window -t $session:2 -n 'develop'
    tmux send-keys -t 'develop' 'nix develop' C-m
fi
tmux attach-session -t $session:2
