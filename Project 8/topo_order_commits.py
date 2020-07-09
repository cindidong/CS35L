# Keep the function signature,
# but replace its body with your implementation
#
# Note that this is the driver function.
# Please write a well-structured implemention by creating other functions
# each of which has a designated purpose.
#
# As a good programming practice,
# please do not use any script-level variables that are modifiable.
# This is because those variables live on forever once the script is imported,
# and the changes to them will persist across different invocations of the imported functions
import os
import sys
import zlib


class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()
        self.branch = []

    def add_parents(self, parent_hash):
        self.parents.add(parent_hash)

    def add_children(self, children_hash):
        self.children.add(children_hash)

    def add_branch(self, branch_name):
        self.branch.append(branch_name)


def get_to_git(dir_name):
    while dir_name != '/':
        path = dir_name + '/.git'
        is_exist = os.path.exists(path)
        if is_exist:
            return dir_name
        else:
            dir_name = os.path.split(dir_name)[0]
    sys.stderr.write('Not inside a Git repository\n')
    exit(1)


def get_branches(dir_list, folders, branches):
    list_of_files = os.scandir(dir_list)
    if not list_of_files:
        return branches
    for x in list_of_files:
        if x.is_dir():
            new_path = os.path.join(dir_list, x.name)
            get_branches(new_path, folders, branches)
        else:
            path = os.path.join(dir_list, x.name)
            temp_hash = open(path, 'r').read().strip('\n')
            temp_name = path.split("/")
            count = 0
            for h in temp_name:
                count = count + 1
                if h == "heads":
                    break
            new_name = '/'.join(temp_name[count:])
            branches[new_name] = temp_hash


def build_branch_parents(hash_num, path, vertex_id_to_nodes, hash_object_map, visited):
    stack = [hash_num]
    while stack:
        list_parent_hash = []
        curr_hash = stack.pop()
        new_path = path + '/.git/objects/' + curr_hash[:2] + '/' + curr_hash[2:]
        info = zlib.decompress(open(new_path, 'rb').read())
        if info[:6] == b'commit':
            info = info.decode().split('\n')
            for y in info:
                if y[:6] == 'parent':
                    parent_hash = y[7:]
                    list_parent_hash.append(parent_hash)
        if curr_hash not in visited:
            if curr_hash not in hash_object_map:
                temp_commit_object = CommitNode(curr_hash)
                hash_object_map[curr_hash] = temp_commit_object
            else:
                temp_commit_object = hash_object_map[curr_hash]
            vertex_id_to_nodes.append(temp_commit_object)
            visited.append(curr_hash)
            if list_parent_hash:
                for z in sorted(list_parent_hash):
                    stack.append(z)
                    if z not in hash_object_map:
                        parent_commit_object = CommitNode(z)
                        hash_object_map[z] = parent_commit_object
                    else:
                        parent_commit_object = hash_object_map[z]
                    temp_commit_object.add_parents(parent_commit_object)


def get_topo_ordered_commits(vertex_id_to_nodes, root_vertices):
    order = []
    visited = set()  # visited is the union of the gray and black vertices
    gray_stack = []
    stack = list(root_vertices)
    while stack:
        v = stack.pop()
        # what do you do if v has already been visited?
        if v in visited:
            continue
        visited.add(v)
        # while v is not a child of the vertex on the top of the gray stack
        if gray_stack:
            while v not in gray_stack[-1].children:
                g = gray_stack.pop()
                order.append(g)
        gray_stack.append(v)
        for c in v.children:
            # what do you do if v has already been visited?
            if c in visited:
                continue
            stack.append(c)
    # add the rest of the gray stack into order
    while gray_stack:
        i = gray_stack.pop()
        order.append(i)
    return order


def build_objects():
    folders = []
    branches = {}
    path = get_to_git(os.getcwd())
    head_path = path + '/.git/refs/heads/'
    get_branches(head_path, folders, branches)
    vertex_id_to_nodes = []
    hash_object_map = {}
    visited = []
    for x in sorted(branches):
        build_branch_parents(branches[x], path, vertex_id_to_nodes, hash_object_map, visited)
    vertex_id_to_nodes = sorted(vertex_id_to_nodes, key=lambda t: t.commit_hash)
    for x in sorted(branches):
        for y in vertex_id_to_nodes:
            if y.commit_hash == branches[x]:
                y.add_branch(x)
    for x in vertex_id_to_nodes:
        for z in x.parents:
            z.add_children(x)
    root_vertices = []
    for x in vertex_id_to_nodes:
        if not x.parents:
            root_vertices.append(x)
    final_list = get_topo_ordered_commits(vertex_id_to_nodes, sorted(root_vertices, key=lambda h: h.commit_hash))
    i = 0
    flag = 0
    while i < len(final_list):
        if flag == 1:
            print_children_list = []
            for z in final_list[i].children:
                print_children_list.append(z.commit_hash)
            if print_children_list:
                print("=" + ' '.join(sorted(print_children_list)))
            else:
                print("=")
            flag = 0
        if final_list[i].branch:
            print(final_list[i].commit_hash + " " + ' '.join(sorted(final_list[i].branch)))
        else:
            print(final_list[i].commit_hash)
        if i != len(final_list) - 1:
            if final_list[i+1] not in final_list[i].parents:
                print_parents_list = []
                flag = 1
                for y in final_list[i].parents:
                    print_parents_list.append(y.commit_hash)
                if print_parents_list:
                    print(' '.join(sorted(print_parents_list)) + "=\n")
                else:
                    print("=\n")
        i = i + 1


def topo_order_commits():
    build_objects()


if __name__ == '__main__':
    topo_order_commits()
