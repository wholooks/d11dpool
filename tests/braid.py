# A set of tools for manipulating test braids

import json, os, unittest
from copy import copy
from collections import deque, OrderedDict
from functools import cmp_to_key

TEST_CASE_DIR = "braids/"

def make_dag(parents, bead_work=None, description=None):
    """ Make a DAG object which caches the children, geneses, tips, cohorts, and highest work path. """
    dag = {}
    dag["description"]       = description
    dag["parents"]           = parents
    dag["children"]          = reverse(parents)
    dag["geneses"]           = geneses(parents)
    dag["tips"]              = tips(parents, dag["children"])
    dag["cohorts"]           = cohorts(parents)
    dag["bead_work"]         = bead_work if bead_work else {b:1 for b in dag["parents"]}
    dag["work"]              = work(parents, dag["children"], bead_work)
    dag["highest_work_path"] = highest_work_path(parents, dag["children"])
    return dag

def geneses(parents):
    """ Given a dict of {bead: {parents}}, return the set of beads which have no parents. """
    geneses = set()
    for b,p in parents.items():
        if not p: geneses.add(b)
    return geneses

def tips(parents, children=None):
    """ Given a dict of {bead: {parents}}, return the set of beads which have no children. """
    if not children: children = reverse(parents)
    return geneses(children)

def reverse(parents):
    """ Given a dict of {bead: {parents}}, compute the corresponding {bead: {children}} (or
        vice-versa).
    """
    children = {}
    for bead, bparents in parents.items():
        if bead not in children:
            children[bead] = set()

        for p in bparents:
            if p not in children:
                children[p] = set()
            children[p].add(bead)
    return children

def generation(beads, children=None):
    """ Given a set of <beads>, compute the set of all children of all {beads}. """
    if not beads: return set()
    retval = set()
    for b in beads:
        retval |= children[b]
    return retval

def all_ancestors(b, parents, ancestors):
    """ Gets all ancestors for a bead <b>, filling in ancestors of
        any other ancestors encountered, using a recursive
        algorithm.  Assumes b not in parents and b not in ancestors.
    """
    ancestors[b] = set(copy(parents[b]))
    for p in parents[b]:
        if p not in ancestors: all_ancestors(p, parents, ancestors)
        ancestors[b].update(ancestors[p])

def cohorts(parents, children=None, initial_cohort=None):
    """ Given the seed of the next cohort (which is the set of beads one step older, in the next
        cohort), build an ancestor/descendant set for each visited bead.  A new cohort is
        formed if we encounter a set of beads, stepping in the descendant direction, for which
        *all* beads in this cohort are ancestors of the first generation of beads in the next
        cohort.
    """
    children     = reverse(parents) if not children else children
    dag          = {"parents": parents, "children": children, "tips": tips(parents, children)}
    cohort       = initial_cohort or geneses(parents)
    oldcohort    = set()
    head         = copy(cohort)
    tail         = copy(cohort)
    while True :
        ancestors = {h: set() for h in head}            # Don't let head have ancestors to stop ancestor iteration
        cohort    = copy(head)                          # Initial cohort is the head

        while True:                                     # DFS search
            # Calculate new tail
            if not head: return                         # StopIteration and return
            else:
                for b in cohort-oldcohort:
                    tail |= dag["children"][b]          # Add the next generation to the tail
                tail |= cohort ^ oldcohort              # Add any beads in the oldcohort but not in the new cohort
                if cohort & dag["tips"]:                # If there are any tips in cohort, add remaining tips to tail:
                    tail |= dag["tips"]-cohort
                else:
                    tail -= cohort                      # If there are no tips in the cohort subtract off the cohort

            oldcohort = copy(cohort)                    # Copy so we can tell that a new tail hasn't changed anything
                                                        # Changing the tail but not cohort may find new ancestors

            # Calculate ancestors
            for t in tail:                              # Find all ancestors of all beads in the tail
                if t not in ancestors:
                    # 50.6% of CPU time
                    all_ancestors(t, dag["parents"], ancestors)

            # Calculate cohort
            cohort = set()
            for a in ancestors: cohort |= ancestors[a]  # Union all ancestors with the cohort

            # Check termination cases
            if dag["tips"] <= cohort:                          # Cohort contains all tips
                head = set()                            # StopIteration and return
                break                                   # and yield the current cohort
            elif cohort and all(ancestors[t] == cohort for t in tail): # Standard cohort case
                head = copy(tail)                       # Head of next cohort is the tail from the previous iteration
                break                                   # Yield successful cohort
            elif cohort == oldcohort:                   # Cohort hasn't changed, we may be looping
                if dag["tips"] <= tail:                        # Tail contains all tips but we didn't form a cohort
                    head = set()                        # We also need cohort == oldcohort (yes)
                    cohort |= tail
                    tail = set()
                    break                               # Yield cohort+tail
                else: cohort.update(tail)               # Seems like I'll be doing 2 loops for non-cohort updates
                                                        # This breaks if I omit the cohort == oldcohort condition.
                                                        #elif not tips & tail and not cohort & tips: (doesn't work)
        oldcohort = set()
        yield cohort

def cohort_tail(cohort, parents, children=None):
    """ Given a cohort as a set of beads, compute its tail. If the tail intersects the tips,
        return all tips.
    """
    children = reverse(parents) if not children else children
    return cohort_head(cohort, parents=children, children=parents)

def cohort_head(cohort, parents, children=None):
    """ Given a cohort as a set of beads, compute its head. If the tail intersects the geneses,
        return all geneses.
    """
    children = reverse(parents) if not children else children
    tail = generation(generation(cohort, parents) - cohort, children)
    tips = geneses(parents)
    if not tail or any({t in tips for t in tail}):
        return tips
    return tail

def sub_braid(beads, parents):
    """ Given a set of <beads> (which generally will be a cohort), return the sub-DAG corresponding
        to only those beads. That is, compute the parents dict: {p: {children} for p in beads} such
        that the returned parents dict contains only the beads in <beads> and the parents of all
        beads are only those parents within <beads>.

        The result has the properties:
            geneses(sub_braid(beads, parents)) = cohort_head(beads, parents)
            tips(sub_braid(beads, parents)) = cohort_tail(beads, parents)
            cohorts(sub_braid(beads, parents)) == [beads]
    """
    return {b: {p for p in parents[b] if p in beads} for b in beads}

def work(parents, children=None, bead_work=None):
    """ Find the cumulative work of each bead by adding the work of all ancestor beads.

        <bead_work>: {bead:work} is the self-work for each bead (e.g. 1/x where x is the PoW target)
        With fixed_bead_work=1 per bead, work is equivalent the number of ancestors for each bead.
    """
    if not children: children = reverse(parents)
    fixed_bead_work = 1
    bead_work       = bead_work if bead_work else {b: fixed_bead_work for b in parents}
    previous_work   = 0
    retval          = {} # The cumulative work for each bead
    for c in cohorts(parents):
        sub_parents   = sub_braid(c, parents)    # Parents dict within the cohort
        sub_ancestors = {}                       # Ancestors within the cohort
        head = cohort_head(c, parents, children) # Youngest set of beads in the cohort
        tail = cohort_tail(c, parents, children) # Oldest set of beads in the cohort
        for b in c:
            all_ancestors(b, sub_parents, sub_ancestors)
            retval[b] = previous_work + bead_work[b] + sum([bead_work[a] for a in sub_ancestors[b]])
        # All beads in the next cohort have ALL beads in this cohort as ancestors.
        previous_work += sum([bead_work[b] for b in c])
    return retval

def check_cohort(cohort, parents, children=None):
    """ Check a cohort using check_cohort_ancestors in both directions. """
    children = reverse(parents) if not children else children
    return check_cohort_ancestors(cohort, parents, children) \
            and check_cohort_ancestors(cohort, children, parents)

def check_cohort_ancestors(cohort, parents, children=None):
    """ Check a cohort by determining the set of ancestors of all beads.  This computation is done
        over the ENTIRE DAG since any ancestor could have a long dangling path leading to this
        cohort.  This will not determine if a cohort has valid sub-cohorts since the merging of any
        two or more adjacent cohorts is still a valid cohort.

        This checks in one direction only, looking at the ancestors of `cohort`. To check in the
        other direction, reverse the order of the parents and children arguments.
    """
    children = reverse(parents) if not children else children
    ancestors = dict()
    allancestors = set()
    head = cohort_head(cohort, parents, children)
    for b in cohort:
        all_ancestors(b, parents, ancestors)
        allancestors |= ancestors[b]
    allancestors -= cohort
    if allancestors and generation(allancestors, children) - allancestors != head:
        return False
    return True

def layout(cohort, parents, children=None, weights=None):
    """Create a graphical layout of a cohort on a 2D grid.
    Head beads are placed in column 0, tail beads in the last column.
    Parents are strictly to the left of their children.
    The highest work path is placed on y=0.
    No beads overlap and no edges pass through beads.

    Args:
        cohort: Set of beads to layout
        parents: Dict mapping beads to their set of parent beads
        children: Optional dict mapping beads to their children
        weights: Optional dict mapping beads to their work values

    Returns:
        Dict mapping beads to their [x,y] coordinates
    """
    if children is None:
        children = reverse(parents)
    if weights is None:
        weights = work(parents, children)

    coords = {}
    used_coords = set()
    sub_parents = sub_braid(cohort, parents)
    sub_children = reverse(sub_parents)
    sortkey = work_sort_key(parents, weights)

    # Identify head and tail beads
    head_beads = cohort_head(cohort, parents, children)
    tail_beads = cohort_tail(cohort, parents, children)
    hwpath = highest_work_path(sub_parents, sub_children)
    fixed_beads = set(hwpath) | head_beads | tail_beads
    num_cols = len(hwpath)

    # Place hwpath beads
    for i, bead in enumerate(hwpath):
        coords[bead] = [i, 0]
        used_coords.add((i, 0))

    # Place tail beads first
    y = 0
    for bead in sorted(tail_beads - set(hwpath), key=sortkey, reverse=True):
        if bead in used_coords:  # Don't move beads that are in hwpath
            continue
        while (num_cols - 1, y) in used_coords:
            y += 1
        coords[bead] = [num_cols - 1, y]
        used_coords.add((num_cols - 1, y))
        y += 1

    # Place head beads, preserving position of head/tail intersection
    y = 0
    for bead in sorted(head_beads - set(hwpath) - tail_beads, key=sortkey, reverse=True):
        if bead in used_coords:  # Don't move beads that are in hwpath or tail
            continue
        while (0, y) in used_coords:
            y += 1
        coords[bead] = [0, y]
        used_coords.add((0, y))
        y += 1

    def find_valid_column(bead):
        """Find valid column for bead based on parents and children."""
        parent_cols = [coords[p][0] for p in sub_parents[bead] if p in coords]
        child_cols = [coords[c][0] for c in sub_children[bead] if c in coords]
        min_col = max(parent_cols, default=-1) + 1
        max_col = min(child_cols, default=num_cols) - 1
        return min_col if min_col <= max_col else min_col

    # Process remaining beads in topological order
    remaining = [b for b in cohort if b not in coords]
    while remaining:
        ready = [b for b in remaining if all(p in coords for p in sub_parents[b])]
        if not ready:
            ready = [max(remaining, key=sortkey)]

        for bead in sorted(ready, key=sortkey, reverse=True):
            x = find_valid_column(bead)
            y = 1
            while (x, y) in used_coords:
                y += 1
            coords[bead] = [x, y]
            used_coords.add((x, y))
            remaining.remove(bead)

    def find_intersections():
        """Find columns where edges intersect movable beads efficiently."""
        # Build column index for faster lookup
        beads_by_col = {}
        for bead, (x, y) in coords.items():
            if bead not in fixed_beads:  # Only track movable beads
                beads_by_col.setdefault(x, []).append((bead, y))

        intersections = {}
        checked_pairs = set()

        for child in cohort:
            child_x, child_y = coords[child]
            for parent in sub_parents[child]:
                pair = (parent, child)
                if pair in checked_pairs:
                    continue
                checked_pairs.add(pair)

                parent_x, parent_y = coords[parent]
                if parent_x >= child_x:
                    continue

                min_y = min(parent_y, child_y)
                max_y = max(parent_y, child_y)

                # Check only columns between parent and child
                for x in range(parent_x + 1, child_x):
                    if x not in beads_by_col:
                        continue

                    # Only check movable beads for intersection
                    for bead, y in beads_by_col[x]:
                        if min_y <= y <= max_y:
                            intersections.setdefault(x, set()).add(bead)

        return {k: list(v) for k, v in intersections.items()}

    # Post-process: move intersected beads down
    max_iterations = 10
    iteration = 0
    while iteration < max_iterations:
        intersections = find_intersections()
        if not intersections:
            break

        # Move beads in intersecting columns
        moved = False
        for col, beads in intersections.items():
            if not beads:
                continue

            # Get all movable beads in this column
            col_beads = [(b, coords[b][1]) for b in cohort
                        if coords[b][0] == col and b not in fixed_beads]
            col_beads.sort(key=lambda x: x[1])  # Sort by y coordinate

            # Find minimum y of intersected beads
            min_y = 1 # min(coords[b][1] for b in beads)

            # Move beads down
            for bead, y in col_beads:
                if y >= min_y:
                    used_coords.remove((col, coords[bead][1]))
                    new_y = y + 1
                    while (col, new_y) in used_coords:
                        new_y += 1
                    coords[bead] = [col, new_y]
                    used_coords.add((col, new_y))
                    moved = True

        if not moved:
            break
        iteration += 1

    # Normalize y coordinates
    y_vals = sorted(set(y for _, y in coords.values()))
    y_map = {y: i for i, y in enumerate(y_vals)}
    return {b: [x, y_map[y]] for b, [x, y] in coords.items()}

def bead_cmp(a, b, parents, weights=None):
    """ A custom cmp(b1,b2) function for sorting beads.

        In the event of a tie it resolves the tie in the following order:
        1. Highest work
        2. FUTURE UNIMPLEMENTED: Lowest hash ("luck")
        3. FUTURE UNIMPLEMENTED: Earliest timestamp
        4. Lowest label (block hash) -- this is "earliest" in simulations

        Use this like:

            sorted(beads, key=cmp_to_key(lambda x,y: braid.bead_cmp(x,y,parents,weights)))
    """
    weights = weights if weights else work(parents)
    if weights[a] < weights[b]: return -1               # highest weight
    elif weights[a] > weights[b]: return 1
    # else both have the same work
    elif a > b: return -1                               # lexical order (lowest first)
    elif a < b: return 1
    else: return 0

def work_sort_key(parents, weights=None):
    """ Return a sorting key lambda suitable to feed to python's min, max, sort etc. Note that
        sort() sorts values from lowest to highest. When using work_sort_key it will sort beads from
        lowest work to highest. Use `reverse=True` as an argument to sort() if you want the highest
        work, or reverse the resultant list.

        Use this like:
            sorted(beads, key=max_work_key(parents, weights))   # sorted from lowest work to highest
            max(beads, key=work_sort_key(parents, weights))     # maximum work
            min(beads, key=work_sort_key(parents, weights))     # maximum work
    """
    weights = weights if weights else work(parents)
    return cmp_to_key(lambda a,b: bead_cmp(a,b,parents,weights))

def highest_work_path(parents, children=None, weights=None):
    """ Find the highest work path, by following the highest weights through the DAG as given by
        max_work().
    """
    weights = weights if weights else work(parents)
    key     = cmp_to_key(lambda x,y: bead_cmp(x,y,parents,weights))
    hwpath  = [max(tips(parents, children), key=key)]

    while hwpath[-1] not in geneses(parents):
        gen = generation({hwpath[-1]}, parents)
        hwpath.append(max(generation({hwpath[-1]}, parents),
                          key=work_sort_key(parents, weights)))
    return list(reversed(hwpath))

def load_braid(filename):
    """ Load a JSON file containing a braid.
    """
    with open(filename) as json_data:
        d = json.load(json_data)
        dag = {}
        dag["description"]       = d["description"]
        dag["parents"]           = {int(k): set(v) for k,v in d["parents"].items()}
        dag["children"]          = {int(k): set(v) for k,v in d["children"].items()}
        dag["geneses"]           = set(d["geneses"])
        dag["tips"]              = set(d["tips"])
        dag["cohorts"]           = [set(map(int,c)) for c in d["cohorts"]]
        if "bead_work" not in d or d["bead_work"] == None:
            dag["bead_work"]     = {b:1 for b in dag["parents"]}
        else:
            dag["bead_work"]     = {int(k): v for k,v in d["bead_work"].items()}
        dag["work"]              = {int(k): v for k,v in d["work"].items()}
        dag["highest_work_path"] = d["highest_work_path"]

        json_data.close()
        return dag

def save_braid(parents, filename, description=None):
    """ Save a JSON file containing a braid. It should contain the keys "description", "parents",
        "cohorts", and "workpath"
    """
    with open(filename, 'w') as file:
        dag = make_dag(parents)
        result = OrderedDict([
            ("description", description),
            ("parents", {k: list(v) for k,v in dag["parents"].items()}),
            ("children", {k: list(v) for k,v in dag["children"].items()}),
            ("geneses", list(dag["geneses"])),
            ("tips", list(dag["tips"])),
            ("cohorts", [sorted(list(map(int, c))) for c in dag["cohorts"]]),
            ("bead_work", dag["bead_work"]),
            ("work", dag["work"]),
            ("highest_work_path", dag["highest_work_path"])
        ])
        file.write(json.dumps(result, sort_keys=False, indent=4))
        file.close()

class TestCohortMethods(unittest.TestCase):
    parents1 = {0:set(), 1:{0}, 2:{1}, 3:{2}} # blockchain
    parents2 = {0:set(), 1:set(), 2:{1}, 3:{1}}
    parents3 = {0:set(), 1:set(), 2:set(), 3:{1}, 4:{0}}
    parents4 = {0:set(), 1:set(), 2:set(), 3:{0,1,2}, 4:{0,1,2}, 5:{0,1,2}}
    diamond  = {0:set(), 1:{0}, 2:{0}, 3:{1,2}, 4:{3}}

    def test_geneses1(self):
        self.assertEqual(geneses(self.parents1), {0})

    def test_geneses2(self):
        self.assertEqual(geneses(self.parents2), {0, 1})

    def test_geneses3(self):
        self.assertEqual(geneses(self.parents3), {0, 1, 2})

    def test_geneses_files(self):
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            self.assertEqual(geneses(dag["parents"]), {0})

    def test_tips1(self):
        parents1 = {0:set(), 1:[0], 2:[1], 3:[2]}
        self.assertEqual(tips(parents1), {3})

    def test_tips2(self):
        parents2 = {0:set(), 1:[0], 2:[1], 3:[1]}
        self.assertEqual(tips(parents2), {2,3})

    def test_tips3(self):
        parents3 = {0:set(), 1:set(), 2:set(), 3:[0,1,2], 4:[0,1,2], 5:[0,1,2]}
        self.assertEqual(tips(parents3), {3,4,5})

    def test_reverse(self):
        self.assertEqual(reverse(self.parents4),
                         {0: {3, 4, 5}, 1: {3, 4, 5}, 2: {3, 4, 5}, 3: set(), 4: set(), 5: set()})

    def test_cohorts(self):
        """ Test cohorts for a trivial blockchain-like braid. """
        self.assertEqual(list(cohorts(self.parents1)), [{0}, {1}, {2}, {3}])

    def test_cohorts_files(self):
        """ Test an assortment of example *.braid files. """
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            self.assertEqual(list(cohorts(dag["parents"])), dag["cohorts"])

    def test_cohorts_reversed_files(self):
        """ Does it find the same cohorts in the forward and backward directions? """
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            p = reverse(dag["parents"])
            c = dag["cohorts"]
            c.reverse()
            self.assertEqual(list(cohorts(p)), c, msg="Test file: {filename}")

    def test_highest_work_path(self):
        self.assertEqual(highest_work_path(self.parents1, reverse(self.parents1)), [0,1,2,3])

    def test_higest_work_path_files(self):
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            self.assertEqual(highest_work_path(dag["parents"], dag["children"]),
                             dag["highest_work_path"], msg=f"Test file: {filename}")

    def test_check_cohort_files(self):
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            for c in dag["cohorts"]:
                self.assertEqual(check_cohort(c, dag["parents"], dag["children"]), True, msg=f"Test file: {filename}")

    def test_check_work_files(self):
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            for c in dag["cohorts"]:
                self.assertEqual(dag["work"], work(dag["parents"], dag["children"], dag["bead_work"]),
                                 msg=f"Test file: {filename}")

    def test_sub_braid_files(self):
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            for c in dag["cohorts"]:
                msg = f"Test file: {filename}"
                self.assertEqual(geneses(sub_braid(c, dag["parents"])),
                                 cohort_head(c, dag["parents"], dag["children"]), msg=msg)
                self.assertEqual(tips(sub_braid(c, dag["parents"])),
                                 cohort_tail(c, dag["parents"], dag["children"]), msg=msg)
                self.assertEqual(list(cohorts(sub_braid(c, dag["parents"]))),
                                 [c], msg=msg)

    def test_head_tail_files(self):
        """ Test that cohort_head and cohort_tail do the same thing as geneses and tips. """
        for filename in sorted([filename for filename in os.listdir(TEST_CASE_DIR) if filename.endswith(".braid")]):
            dag = load_braid(filename)
            for c in dag["cohorts"]:
                msg = f"Test file: {filename}"
                self.assertEqual(cohort_head(c, dag["parents"], dag["children"]),
                                 geneses(sub_braid(c, dag["parents"])), msg=msg)
                self.assertEqual(cohort_tail(c, dag["parents"], dag["children"]),
                                 tips(sub_braid(c, dag["parents"])), msg=msg)

if __name__ == "__main__":
    unittest.main()
